#ifndef DEFAULT_RENDER_LOOP_H
#define DEFAULT_RENDER_LOOP_H

#include <functional>


#define GLEW_STATIC
#include <iostream>
#include <memory>
#include <exception>
#include "GL/glew.h"
// Uncomment your platform
#define WIN32_LEAN_AND_MEAN
#define OVR_OS_WIN32
//#define OVR_OS_MAC
//#define OVR_OS_LINUX
#include <windows.h>
#include "../Src/Kernel/OVR_Math.h"
#include "../Src/OVR_CAPI.h"
#include "../Src/OVR_CAPI_GL.h"
#include <GLFW/glfw3.h>


//////////////////////////////////////////////////////////////////////
//
// The OVR types header contains the OS detection macros:
//  OVR_OS_WIN32, OVR_OS_MAC, OVR_OS_LINUX (and others)
//

#if (defined(WIN64) || defined(WIN32))
#define OS_WIN
#elif (defined(__APPLE__))
#define OS_OSX
#else
#define OS_LINUX
#endif

// Windows has a non-standard main function prototype
#ifdef OS_WIN
#define MAIN_DECL int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
#define MAIN_DECL int main(int argc, char ** argv)
#endif

#if defined(OS_WIN)
#define ON_WINDOWS(runnable) runnable()
#define ON_MAC(runnable)
#define ON_LINUX(runnable)
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#undef NOMINMAX
#elif defined(OS_OSX)
#define ON_WINDOWS(runnable) 
#define ON_MAC(runnable) runnable()
#define ON_LINUX(runnable)
#elif defined(OS_LINUX)
#define ON_WINDOWS(runnable) 
#define ON_MAC(runnable) 
#define ON_LINUX(runnable) runnable()
#endif

#define __STDC_FORMAT_MACROS 1
#define GLM_FORCE_RADIANS

#define FAIL(X) throw std::runtime_error(X)


// For interaction with the Rift, on some platforms we require
// native window handles, so we need to define some symbols and
// include a special header to allow us to get them from GLFW

#if defined(OS_WIN)
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#elif defined(OS_OSX)
#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL
#elif defined(OS_LINUX)
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#endif

#include <GLFW/glfw3native.h>


///////////////////////////////////////////////////////////////////////////////
//
// GLM is a C++ math library meant to mirror the syntax of GLSL 
//

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>


// Import the most commonly used types into the default namespace
using glm::ivec3;
using glm::ivec2;
using glm::uvec2;
using glm::mat3;
using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::quat;
using glm::max;

GLFWwindow * createWindow(const uvec2 & size, const ivec2 & position = ivec2(INT_MIN)) {
	GLFWwindow * window = glfwCreateWindow(size.x, size.y, "glfw", nullptr, nullptr);
	if (!window) {
		FAIL("Unable to create rendering window");
	}
	if ((position.x > INT_MIN) && (position.y > INT_MIN)) {
		glfwSetWindowPos(window, position.x, position.y);

	}
	return window;
}


GLFWwindow* generateWindow(int width = 1280, int height = 720){
	if (!glfwInit())
		exit(EXIT_FAILURE);

	bool debug = false;

	ovr_Initialize();

	ovrHmd hmd = ovrHmd_Create(0);

	if (hmd == NULL)
	{
		hmd = ovrHmd_CreateDebug(ovrHmd_DK1);

		debug = true;
	}

	GLFWwindow * window = nullptr;

	glm::ivec2 & outPosition = glm::ivec2(hmd->WindowsPos.x, hmd->WindowsPos.y);
	glm::uvec2 & outSize = glm::uvec2(hmd->Resolution.w, hmd->Resolution.h);

	window = createWindow(outSize, outPosition);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}


	glfwMakeContextCurrent(window);

	//SDL_Window *window = SDL_CreateWindow("Oculus Rift SDL2 OpenGL Demo", x, y, w, h, flags);

	//SDL_GLContext context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;

	glewInit();

	OVR::Sizei recommendedTex0Size = ovrHmd_GetFovTextureSize(hmd, ovrEye_Left, hmd->DefaultEyeFov[0], 1.0f);
	OVR::Sizei recommendedTex1Size = ovrHmd_GetFovTextureSize(hmd, ovrEye_Right, hmd->DefaultEyeFov[1], 1.0f);
	OVR::Sizei renderTargetSize;
	renderTargetSize.w = recommendedTex0Size.w + recommendedTex1Size.w;
	renderTargetSize.h = glm::max(recommendedTex0Size.h, recommendedTex1Size.h);

	GLuint frameBuffer;
	glGenFramebuffers(1, &frameBuffer);

	GLuint texture;
	glGenTextures(1, &texture);

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, renderTargetSize.w, renderTargetSize.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);

	GLuint renderBuffer;
	glGenRenderbuffers(1, &renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, renderTargetSize.w, renderTargetSize.h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		glDeleteFramebuffers(1, &frameBuffer);
		glDeleteTextures(1, &texture);
		glDeleteRenderbuffers(1, &renderBuffer);

		//SDL_GL_DeleteContext(context);

		glfwDestroyWindow(window);
		//SDL_DestroyWindow(window);

		ovrHmd_Destroy(hmd);

		ovr_Shutdown();

		glfwTerminate();
		//SDL_Quit();

		return 0;
	}

	renderTargetSize.w = hmd->Resolution.w;
	renderTargetSize.h = hmd->Resolution.h;

	ovrFovPort eyeFov[2] = { hmd->DefaultEyeFov[0], hmd->DefaultEyeFov[1] };

	ovrRecti eyeRenderViewport[2];
	eyeRenderViewport[0].Pos = OVR::Vector2i(0, 0);
	eyeRenderViewport[0].Size = OVR::Sizei(renderTargetSize.w / 2, renderTargetSize.h);
	eyeRenderViewport[1].Pos = OVR::Vector2i((renderTargetSize.w + 1) / 2, 0);
	eyeRenderViewport[1].Size = eyeRenderViewport[0].Size;

	ovrGLTexture eyeTexture[2];
	eyeTexture[0].OGL.Header.API = ovrRenderAPI_OpenGL;
	eyeTexture[0].OGL.Header.TextureSize = renderTargetSize;
	eyeTexture[0].OGL.Header.RenderViewport = eyeRenderViewport[0];
	eyeTexture[0].OGL.TexId = texture;

	eyeTexture[1] = eyeTexture[0];
	eyeTexture[1].OGL.Header.RenderViewport = eyeRenderViewport[1];

	ovrGLConfig cfg;
	cfg.OGL.Header.API = ovrRenderAPI_OpenGL;
	//cfg.OGL.Header.BackBufferSize = Sizei(hmd->Resolution.w, hmd->Resolution.h);

	cfg.OGL.Header.BackBufferSize = OVR::Sizei(hmd->Resolution.w, hmd->Resolution.h);

	cfg.OGL.Header.Multisample = 1;

#if defined(OVR_OS_WIN32)
	if (!(hmd->HmdCaps & ovrHmdCap_ExtendDesktop))
		ovrHmd_AttachToWindow(hmd, window, NULL, NULL);

	//cfg.OGL.Window = info.info.win.window;
	cfg.OGL.Window = glfwGetWin32Window(window);
	cfg.OGL.DC = NULL;
#elif defined(OVR_OS_LINUX)
	cfg.OGL.Disp = info.info.x11.display;
	cfg.OGL.Win = info.info.x11.window;
#endif

	ovrEyeRenderDesc eyeRenderDesc[2];

	ovrHmd_ConfigureRendering(hmd, &cfg.Config, ovrDistortionCap_Chromatic | ovrDistortionCap_Vignette | ovrDistortionCap_TimeWarp | ovrDistortionCap_Overdrive, eyeFov, eyeRenderDesc);

	ovrHmd_SetEnabledCaps(hmd, ovrHmdCap_LowPersistence | ovrHmdCap_DynamicPrediction);

	ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection | ovrTrackingCap_Position, 0);

	const GLchar *vertexShaderSource[] = {
		"#version 150\n"
		"uniform mat4 MVPMatrix;\n"
		"in vec3 position;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = MVPMatrix * vec4(position, 1.0);\n"
		"}"
	};

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	const GLchar *fragmentShaderSource[] = {
		"#version 150\n"
		"out vec4 outputColor;\n"
		"void main()\n"
		"{\n"
		"    outputColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
		"}"
	};

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glUseProgram(program);

	GLuint MVPMatrixLocation = glGetUniformLocation(program, "MVPMatrix");
	GLuint positionLocation = glGetAttribLocation(program, "position");

	GLuint vertexArray;
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	GLfloat vertices[] = {
		0.0f, 1.0f, -2.0f,
		-1.0f, -1.0f, -2.0f,
		1.0f, -1.0f, -2.0f
	};

	GLuint positionBuffer;
	glGenBuffers(1, &positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionLocation);

	glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	bool running = true;

	while (running == true)
	{
		// key definitions
		ovrFrameTiming frameTiming = ovrHmd_BeginFrame(hmd, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ovrPosef eyeRenderPose[2];
		for (int eyeIndex = 0; eyeIndex < ovrEye_Count; eyeIndex++)
		{
			ovrEyeType eye = hmd->EyeRenderOrder[eyeIndex];
			eyeRenderPose[eye] = ovrHmd_GetHmdPosePerEye(hmd, eye);
			OVR::Matrix4f MVPMatrix = OVR::Matrix4f(ovrMatrix4f_Projection(eyeRenderDesc[eye].Fov, 0.01f, 10000.0f, true)) * OVR::Matrix4f::Translation(eyeRenderDesc[eye].HmdToEyeViewOffset) * OVR::Matrix4f(OVR::Quatf(eyeRenderPose[eye].Orientation).Inverted());
			glUniformMatrix4fv(MVPMatrixLocation, 1, GL_FALSE, &MVPMatrix.Transposed().M[0][0]);
			glViewport(eyeRenderViewport[eye].Pos.x, eyeRenderViewport[eye].Pos.y, eyeRenderViewport[eye].Size.w, eyeRenderViewport[eye].Size.h);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		glBindVertexArray(0);
		ovrHmd_EndFrame(hmd, eyeRenderPose, &eyeTexture[0].Texture);
		glBindVertexArray(vertexArray);
	}

	glDeleteVertexArrays(1, &vertexArray);
	glDeleteBuffers(1, &positionBuffer);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(program);

	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteTextures(1, &texture);
	glDeleteRenderbuffers(1, &renderBuffer);

	glfwDestroyWindow(window);

	ovrHmd_Destroy(hmd);

	ovr_Shutdown();

	glfwTerminate();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	return window;
}


ovrSizei fromGlm(const uvec2 & v) {
	ovrSizei result;
	result.w = v.x;
	result.h = v.y;
	return result;
}


void render(GLFWwindow* window, std::function<void(double)> loop) {
	float lastTime = 0.0;
	while (!glfwWindowShouldClose(window)) {

		float currentTime = glfwGetTime();
		loop(currentTime - lastTime );
		lastTime = currentTime;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}

void setKeyCallback(GLFWwindow* window, std::function<void(int, int, int, int)> func) {
	static std::function<void(int, int, int, int)> func_bounce = func;
	glfwSetKeyCallback(window, [](GLFWwindow* w, int k, int s, int a, int m) {
		func_bounce(k, s, a, m);
	});
}

void setMouseButtonCallback(GLFWwindow* window, std::function<void(int, int, int)> func) {
	static std::function<void(int, int, int)> func_bounce = func;
	glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int b, int a, int m) {
		func_bounce(b, a, m);
	});
}

void setCharCallback(GLFWwindow* window, std::function<void(unsigned int)> func) {
	static std::function<void(unsigned int)> func_bounce = func;
	glfwSetCharCallback(window, [](GLFWwindow* w, unsigned int c) {
		func_bounce(c);
	});
}

void setCursorPosCallback(GLFWwindow* window, std::function<void(double, double)> func) {
	static std::function<void(double, double)> func_bounce = func;
	glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {
		func_bounce(x, y);
	});
}

void setScrollCallback(GLFWwindow* window, std::function<void(double, double)> func) {
	static std::function<void(double, double)> func_bounce = func;
	glfwSetScrollCallback(window, [](GLFWwindow* w, double x, double y) {
		func_bounce(x, y);
	});
}

void setCursorEnterCallback(GLFWwindow* window, std::function<void(int)> func) {
	static std::function<void(int)> func_bounce = func;
	glfwSetCursorEnterCallback(window, [](GLFWwindow* w, int e) {
		func_bounce(e);
	});
}

#endif // DEFAULT_RENDER_LOOP_H
