#include <GL/glew.h>
#if defined(_WIN32)
#include <Windows.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#elif defined(__linux__)
#include <X11/X.h>
#include <X11/extensions/Xrandr.h>
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#endif
#include <GLFW/glfw3.h>
#if !defined(__APPLE__)
#include <GLFW/glfw3native.h>
#endif
#include <OVR.h>
#include <../Src/OVR_CAPI_GL.h>
#include "glm\glm.hpp"
#include "ShaderTools/RenderPass.h"
#include "Compression/TextureTools.h"
#include <functional>

const bool l_MultiSampling = false;
const bool l_Spin = false;

int g_DistortionCaps = 0
| ovrDistortionCap_Vignette
| ovrDistortionCap_Chromatic
| ovrDistortionCap_Overdrive
// | ovrDistortionCap_TimeWarp // Turning this on gives ghosting???
;

ovrHmd g_Hmd;
ovrGLConfig g_Cfg;
ovrEyeRenderDesc g_EyeRenderDesc[2];
ovrVector3f g_EyeOffsets[2];
ovrPosef g_EyePoses[2];
ovrTexture g_EyeTextures[2];
OVR::Matrix4f g_ProjectionMatrici[2];
OVR::Sizei g_RenderTargetSize;
ovrVector3f g_CameraPosition;

// Disables Oculus Screen and renders scene in external window with oculus tracking
static const bool DEVELOPMODE = FALSE;

// Create the FBO being a single one for both eyes (this is open for debate)...
GLuint l_FBOId;
// Create Depth Buffer...
GLuint l_DepthBufferId;
// The texture we're going to render to...
GLuint l_TextureId;



static void ErrorCallback(int p_Error, const char* p_Description)
{
	fputs(p_Description, stderr);
}

static void RecenterPose()
{
	ovrHmd_RecenterPose(g_Hmd);
}

static void KeyCallback(GLFWwindow* p_Window, int p_Key, int p_Scancode, int p_Action, int p_Mods)
{
	//printf("Press Action: %d", p_Action);

	if (p_Action == GLFW_PRESS || p_Action == GLFW_REPEAT)
	{
		switch (p_Key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(p_Window, GL_TRUE);
			break;
		case GLFW_KEY_R:
			ovrHmd_RecenterPose(g_Hmd);
			break;
		case GLFW_KEY_UP:
			g_CameraPosition.z += 0.1f;
			break;
		case GLFW_KEY_DOWN:
			g_CameraPosition.z -= 0.1f;
			break;
		case GLFW_KEY_LEFT:
			g_CameraPosition.x += 0.1f;
			break;
		case GLFW_KEY_RIGHT:
			g_CameraPosition.x -= 0.1f;
			break;
		case GLFW_KEY_W:
			g_CameraPosition.y -= 0.1f;
			break;
		case GLFW_KEY_S:
			g_CameraPosition.y += 0.1f;
			break;
		case GLFW_KEY_A:
			g_CameraPosition.x += 0.1f;
			break;
		case GLFW_KEY_D:
			g_CameraPosition.x -= 0.1f;
			break;
		}

		// Remove HSW on every key...
		ovrHSWDisplayState l_HasWarningState;
		ovrHmd_GetHSWDisplayState(g_Hmd, &l_HasWarningState);
		if (l_HasWarningState.Displayed) ovrHmd_DismissHSWDisplay(g_Hmd);
	}
}

static void WindowSizeCallback(GLFWwindow* p_Window, int p_Width, int p_Height)
{
	if (p_Width>0 && p_Height>0)
	{
		g_Cfg.OGL.Header.BackBufferSize.w = p_Width;
		g_Cfg.OGL.Header.BackBufferSize.h = p_Height;

		ovrBool l_ConfigureResult = ovrHmd_ConfigureRendering(g_Hmd, &g_Cfg.Config, g_DistortionCaps, g_Hmd->MaxEyeFov, g_EyeRenderDesc);
		if (!l_ConfigureResult)
		{
			printf("Configure failed.\n");
			exit(EXIT_FAILURE);
		}
	}
}

static void SetOpenGLState(void)
{
	// Some state...
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (l_MultiSampling) glEnable(GL_MULTISAMPLE); else glDisable(GL_MULTISAMPLE);
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
}

glm::vec2 getResolution(GLFWwindow* window) {
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	return glm::vec2(float(w), float(h));
}

glm::mat4 toGlm(const ovrMatrix4f & om) {
	return glm::transpose(glm::make_mat4(&om.M[0][0]));
}

glm::mat4 toGlm(const ovrFovPort & fovport, float nearPlane = 0.1f, float farPlane = 10000.0f) {
	return toGlm(ovrMatrix4f_Projection(fovport, nearPlane, farPlane, true));
}

glm::vec3 toGlm(const ovrVector3f & ov) {
	return glm::make_vec3(&ov.x);
}

glm::vec2 toGlm(const ovrVector2f & ov) {
	return glm::make_vec2(&ov.x);
}

glm::uvec2 toGlm(const ovrSizei & ov) {
	return glm::uvec2(ov.w, ov.h);
}

glm::quat toGlm(const ovrQuatf & oq) {
	return glm::make_quat(&oq.x);
}

glm::mat4 toGlm(const ovrPosef & op) {
	glm::mat4 orientation = glm::mat4_cast(toGlm(op.Orientation));
	glm::mat4 translation = glm::translate(glm::mat4(), toGlm(op.Position));
	return translation * orientation;
}

inline void render(GLFWwindow* window, std::function<void(double, glm::mat4 projection, glm::mat4 view)> loop, std::function<void()> performHHF){
	float lastTime = 0.0;
	unsigned int l_FrameIndex = 0;

	while (!glfwWindowShouldClose(window)){
		double currentTime = glfwGetTime();

		// Begin the frame...
		ovrHmd_BeginFrame(g_Hmd, l_FrameIndex);

		// Get eye poses for both the left and the right eye. g_EyePoses contains all Rift information: orientation, positional tracking and
		// the IPD in the form of the input variable g_EyeOffsets.
		ovrHmd_GetEyePoses(g_Hmd, l_FrameIndex, g_EyeOffsets, g_EyePoses, NULL);

		// Bind the FBO...
		glBindFramebuffer(GL_FRAMEBUFFER, l_FBOId);

		// Clear...
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glClear(GL_COLOR_BUFFER_BIT);
		for (int l_EyeIndex = 0; l_EyeIndex<ovrEye_Count; l_EyeIndex++)
		{
			ovrEyeType l_Eye = g_Hmd->EyeRenderOrder[l_EyeIndex];

			glViewport(
				g_EyeTextures[l_Eye].Header.RenderViewport.Pos.x,
				g_EyeTextures[l_Eye].Header.RenderViewport.Pos.y,
				g_EyeTextures[l_Eye].Header.RenderViewport.Size.w,
				g_EyeTextures[l_Eye].Header.RenderViewport.Size.h
				);

			OVR::Quatf l_Orientation = OVR::Quatf(g_EyePoses[l_Eye].Orientation);
			OVR::Matrix4f l_ModelViewMatrix = OVR::Matrix4f(l_Orientation.Inverted());

			glm::mat4 view = toGlm(l_ModelViewMatrix);
			glm::mat4 projection = toGlm(g_ProjectionMatrici[l_Eye]);

			//ovrTrackingState ts = ovrHmd_GetTrackingState(g_Hmd, 0.0f);
			//if (ts.StatusFlags & (ovrStatus_PositionTracked)){}

			// Translation due to positional tracking (DK2) and IPD...
			view = glm::translate(view, glm::vec3(-g_EyePoses[l_Eye].Position.x, -g_EyePoses[l_Eye].Position.y, -g_EyePoses[l_Eye].Position.z));
			
			// Move the world forward a bit to show the scene in front of us...
			view = glm::translate(view, glm::vec3(g_CameraPosition.x, g_CameraPosition.y, g_CameraPosition.z));

			loop(currentTime - lastTime, projection, view);

		}
		glViewport(0,0, g_RenderTargetSize.w, g_RenderTargetSize.h);
		// performs hhf and writes to oculus texture
		performHHF();

		// Back to the default framebuffer...
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Do everything, distortion, front/back buffer swap...
		ovrHmd_EndFrame(g_Hmd, g_EyePoses, g_EyeTextures);

		++l_FrameIndex;

		lastTime = currentTime;

		glfwPollEvents();
	}

	// Clean up FBO...
	glDeleteRenderbuffers(1, &l_DepthBufferId);
	glDeleteTextures(1, &l_TextureId);
	glDeleteFramebuffers(1, &l_FBOId);

	// Clean up Oculus...
	ovrHmd_Destroy(g_Hmd);
	ovr_Shutdown();

	// Clean up window...
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}



GLFWwindow* generateWindow(int width = 1280, int height = 720) {
	printf("[R] to recenter, [Esc] to quit, dismiss the HSW with any key (after the hidden timer runs out)...\n");

	// Initialize LibOVR...
	ovr_Initialize();

	// Check for attached head mounted display...
	g_Hmd = ovrHmd_Create(0);
	if (!g_Hmd)
	{
		printf("No Oculus Rift device attached, using virtual version...\n");
		g_Hmd = ovrHmd_CreateDebug(ovrHmd_DK2);
	}

	// Create a window...
	GLFWwindow* window;

	glfwSetErrorCallback(ErrorCallback);

	if (!glfwInit()) exit(EXIT_FAILURE);

	if (l_MultiSampling) glfwWindowHint(GLFW_SAMPLES, 4); else glfwWindowHint(GLFW_SAMPLES, 0);

	// Check to see if we are running in "Direct" or "Extended Desktop" mode...
	bool directMode = ((g_Hmd->HmdCaps & ovrHmdCap_ExtendDesktop) == 0);

	GLFWmonitor* l_Monitor;
	ovrSizei l_ClientSize;
	if (directMode)
	{
		printf("Running in \"Direct\" mode...\n");
		l_Monitor = NULL;

		l_ClientSize.w = g_Hmd->Resolution.w / 2; // Something reasonable, smaller, but maintain aspect ratio...
		l_ClientSize.h = g_Hmd->Resolution.h / 2; // Something reasonable, smaller, but maintain aspect ratio...

	}
	else // Extended Desktop mode...
	{
		printf("Running in \"Extended Desktop\" mode...\n");
		int l_Count;
		GLFWmonitor** l_Monitors = glfwGetMonitors(&l_Count);
		switch (l_Count)
		{
		case 0:
			printf("No monitors found, exiting...\n");
			exit(EXIT_FAILURE);
			break;
		case 1:
			printf("Two monitors expected, found only one, using primary...\n");
			l_Monitor = glfwGetPrimaryMonitor();
			break;
		case 2:
			printf("Two monitors found, using second monitor...\n");
			l_Monitor = l_Monitors[1];
			break;
		default:
			printf("More than two monitors found, using second monitor...\n");
			l_Monitor = l_Monitors[1];
		}

		l_ClientSize.w = g_Hmd->Resolution.w; // 1920 for DK2...
		l_ClientSize.h = g_Hmd->Resolution.h; // 1080 for DK2...
	}

	// Create the window based on the parameters set above...
	window = glfwCreateWindow(l_ClientSize.w, l_ClientSize.h, "GLFW Oculus Rift Test", l_Monitor, NULL);

	// Check if window creation was succesfull...
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Attach the window in "Direct Mode"...
#if defined(_WIN32)
	if (directMode && !DEVELOPMODE)
	{
		ovrBool l_AttachResult = ovrHmd_AttachToWindow(g_Hmd, glfwGetWin32Window(window), NULL, NULL);
		if (!l_AttachResult)
		{
			printf("Could not attach to window...");
			exit(EXIT_FAILURE);
		}
	}
#endif

	// Make the context current for this window...
	glfwMakeContextCurrent(window);

	// Don't forget to initialize Glew, turn glewExperimental on to avoid problem fetching function pointers...
	glewExperimental = GL_TRUE;
	GLenum l_GlewResult = glewInit();
	if (l_GlewResult != GLEW_OK)
	{
		printf("glewInit() error.\n");
		exit(EXIT_FAILURE);
	}

	// Print some info about the OpenGL context...
	int l_Major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
	int l_Minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
	int l_Profile = glfwGetWindowAttrib(window, GLFW_OPENGL_PROFILE);
	printf("OpenGL: %d.%d ", l_Major, l_Minor);
	if (l_Major >= 3) // Profiles introduced in OpenGL 3.0...
	{
		if (l_Profile == GLFW_OPENGL_COMPAT_PROFILE) printf("GLFW_OPENGL_COMPAT_PROFILE\n"); else printf("GLFW_OPENGL_CORE_PROFILE\n");
	}
	printf("Vendor: %s\n", (char*)glGetString(GL_VENDOR));
	printf("Renderer: %s\n", (char*)glGetString(GL_RENDERER));

	// Create some lights, materials, etc...
	SetOpenGLState();

	// Find out what the texture sizes should be for each eye separately first...
	ovrSizei l_EyeTextureSizes[2];

	l_EyeTextureSizes[ovrEye_Left] = ovrHmd_GetFovTextureSize(g_Hmd, ovrEye_Left, g_Hmd->MaxEyeFov[ovrEye_Left], 1.0f);
	l_EyeTextureSizes[ovrEye_Right] = ovrHmd_GetFovTextureSize(g_Hmd, ovrEye_Right, g_Hmd->MaxEyeFov[ovrEye_Right], 1.0f);

	// Combine for one texture for both eyes...
	g_RenderTargetSize.w = l_EyeTextureSizes[ovrEye_Left].w + l_EyeTextureSizes[ovrEye_Right].w;
	g_RenderTargetSize.h = (l_EyeTextureSizes[ovrEye_Left].h>l_EyeTextureSizes[ovrEye_Right].h ? l_EyeTextureSizes[ovrEye_Left].h : l_EyeTextureSizes[ovrEye_Right].h);


	glGenFramebuffers(1, &l_FBOId);
	glBindFramebuffer(GL_FRAMEBUFFER, l_FBOId);


	glGenTextures(1, &l_TextureId);
	// "Bind" the newly created texture : all future texture functions will modify this texture...
	glBindTexture(GL_TEXTURE_2D, l_TextureId);
	// Give an empty image to OpenGL (the last "0")
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, g_RenderTargetSize.w, g_RenderTargetSize.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	// Linear filtering...
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	glGenRenderbuffers(1, &l_DepthBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, l_DepthBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, g_RenderTargetSize.w, g_RenderTargetSize.h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, l_DepthBufferId);

	// Set the texture as our colour attachment #0...
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, l_TextureId, 0);

	// Set the list of draw buffers...
	GLenum l_GLDrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, l_GLDrawBuffers); // "1" is the size of DrawBuffers

	// Check if everything is OK...
	GLenum l_Check = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	if (l_Check != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("There is a problem with the FBO.\n");
		exit(EXIT_FAILURE);
	}

	// Unbind...
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Setup textures for each eye...

	// Left eye...
	g_EyeTextures[ovrEye_Left].Header.API = ovrRenderAPI_OpenGL;
	g_EyeTextures[ovrEye_Left].Header.TextureSize = g_RenderTargetSize;
	g_EyeTextures[ovrEye_Left].Header.RenderViewport.Pos.x = 0;
	g_EyeTextures[ovrEye_Left].Header.RenderViewport.Pos.y = 0;
	g_EyeTextures[ovrEye_Left].Header.RenderViewport.Size = l_EyeTextureSizes[ovrEye_Left];
	((ovrGLTexture&)(g_EyeTextures[ovrEye_Left])).OGL.TexId = l_TextureId;

	// Right eye (mostly the same as left but with the viewport on the right side of the texture)...
	g_EyeTextures[ovrEye_Right] = g_EyeTextures[ovrEye_Left];
	g_EyeTextures[ovrEye_Right].Header.RenderViewport.Pos.x = (g_RenderTargetSize.w + 1) / 2;
	g_EyeTextures[ovrEye_Right].Header.RenderViewport.Pos.y = 0;

	// Oculus Rift eye configurations...
	g_Cfg.OGL.Header.API = ovrRenderAPI_OpenGL;
	g_Cfg.OGL.Header.BackBufferSize.w = l_ClientSize.w;
	g_Cfg.OGL.Header.BackBufferSize.h = l_ClientSize.h;
	g_Cfg.OGL.Header.Multisample = (l_MultiSampling ? 1 : 0);
#if defined(_WIN32)
	g_Cfg.OGL.Window = glfwGetWin32Window(window);
	g_Cfg.OGL.DC = GetDC(g_Cfg.OGL.Window);
#elif defined(__linux__)
	l_Cfg.OGL.Win = glfwGetX11Window(l_Window);
	l_Cfg.OGL.Disp = glfwGetX11Display();
#endif

	// Enable capabilities...
	// ovrHmd_SetEnabledCaps(g_Hmd, ovrHmdCap_LowPersistence | ovrHmdCap_DynamicPrediction);

	ovrBool l_ConfigureResult = ovrHmd_ConfigureRendering(g_Hmd, &g_Cfg.Config, g_DistortionCaps, g_Hmd->MaxEyeFov, g_EyeRenderDesc);
	if (!l_ConfigureResult)
	{
		printf("Configure failed.\n");
		exit(EXIT_FAILURE);
	}

	// Start the sensor which provides the Rift’s pose and motion...
	uint32_t l_SupportedSensorCaps = ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection | ovrTrackingCap_Position;
	uint32_t l_RequiredTrackingCaps = 0;
	ovrBool l_TrackingResult = ovrHmd_ConfigureTracking(g_Hmd, l_SupportedSensorCaps, l_RequiredTrackingCaps);
	if (!l_TrackingResult)
	{
		printf("Could not start tracking...");
		exit(EXIT_FAILURE);
	}

	// Projection matrici for each eye will not change at runtime, we can set them here...
	g_ProjectionMatrici[ovrEye_Left] = ovrMatrix4f_Projection(g_EyeRenderDesc[ovrEye_Left].Fov, 0.1f, 10000.0f, true);
	g_ProjectionMatrici[ovrEye_Right] = ovrMatrix4f_Projection(g_EyeRenderDesc[ovrEye_Right].Fov, 0.1f, 10000.0f, true);

	// IPD offset values will not change at runtime, we can set them here...
	g_EyeOffsets[ovrEye_Left] = g_EyeRenderDesc[ovrEye_Left].HmdToEyeViewOffset;
	g_EyeOffsets[ovrEye_Right] = g_EyeRenderDesc[ovrEye_Right].HmdToEyeViewOffset;

	// Initial camera position...
	g_CameraPosition.x = 0.0f;
	g_CameraPosition.y = 0.0f;
	g_CameraPosition.z = 1.0f;

	// Remove HSW on every key...
	ovrHSWDisplayState l_HasWarningState;
	ovrHmd_GetHSWDisplayState(g_Hmd, &l_HasWarningState);
	if (l_HasWarningState.Displayed) ovrHmd_DismissHSWDisplay(g_Hmd);

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetWindowSizeCallback(window, WindowSizeCallback);

	// Do a single recenter to calibrate orientation to current state of the Rift...
	ovrHmd_RecenterPose(g_Hmd);
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	return window;
}

/**
* @brief Registers a keyboard button callback function
* @details Pass a function pointer to your keyboard button callback function
*          and it will be registered with the given window. The keyboard
*          function gets called, when a button of the keyboard gets pressed.
*
* @param window The window to register the callback to
* @param func The function pointer of the callback function to register
*/
void setKeyCallback(GLFWwindow* window, std::function<void(int, int, int, int)> func) {
	static std::function<void(int, int, int, int)> func_bounce = func;
	glfwSetKeyCallback(window, [](GLFWwindow* w, int k, int s, int a, int m) {
		func_bounce(k, s, a, m);
	});
}

/**
* @brief Registers a mouse button callback function
* @details Pass a function pointer to your mouse button callback function
*          and it will be registered with the given window. The mouse button
*          function gets called, when a button of the mouse gets pressed.
*
* @param window The window to register the callback to
* @param func The function pointer of the callback function to register
*/
void setMouseButtonCallback(GLFWwindow* window, std::function<void(int, int, int)> func) {
	static std::function<void(int, int, int)> func_bounce = func;
	glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int b, int a, int m) {
		func_bounce(b, a, m);
	});
}

/**
* @brief Registers a character callback function
* @details Pass a function pointer to your character callback function and it
*          will be registered with the given window. The character function
*          gets called, when a single unicode character is input.
*
* @param window The window to register the callback to
* @param func The function pointer of the callback function to register
*/
void setCharCallback(GLFWwindow* window, std::function<void(unsigned int)> func) {
	static std::function<void(unsigned int)> func_bounce = func;
	glfwSetCharCallback(window, [](GLFWwindow* w, unsigned int c) {
		func_bounce(c);
	});
}

/**
* @brief Registers a cursor position callback function
* @details Pass a function pointer to your cursor position callback function
*          and it will be registered with the given window. The cursor position
*          function gets called, when the cursor gets moved on an active
*          window.
*
* @param window The window to register the callback to
* @param func The function pointer of the callback function to register
*/
void setCursorPosCallback(GLFWwindow* window, std::function<void(double, double)> func) {
	static std::function<void(double, double)> func_bounce = func;
	glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {
		func_bounce(x, y);
	});
}

/**
* @brief Registers a scroll callback function
* @details Pass a function pointer to your scroll callback function and it will
*          be registered with the given window. The scroll function gets called
*          when the mouse scroll wheel is used on an active window.
*
* @param window The window to register the callback to
* @param func The function pointer of the callback function to register
*/
void setScrollCallback(GLFWwindow* window, std::function<void(double, double)> func) {
	static std::function<void(double, double)> func_bounce = func;
	glfwSetScrollCallback(window, [](GLFWwindow* w, double x, double y) {
		func_bounce(x, y);
	});
}

/**
* @brief Registers a cursor enter callback function
* @details Pass a function pointer to your cursor enter callback function and
*          it will be registered with the given window. The cursor enter
*          function gets called when entering the window area with the mouse.
*
* @param window The window to register the callback to
* @param func The function pointer of the callback function to register
*/
void setCursorEnterCallback(GLFWwindow* window, std::function<void(int)> func) {
	static std::function<void(int)> func_bounce = func;
	glfwSetCursorEnterCallback(window, [](GLFWwindow* w, int e) {
		func_bounce(e);
	});
}

/**
* @brief Returns the windows width
*
* @param window Window to get the width of
* @return The width of the given window
*/
int getWidth(GLFWwindow* window) {
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	return w;
}

/**
* @brief Returns the windows height
*
* @param window Window to get the height of
* @return The height of the given window
*/
int getHeight(GLFWwindow* window) {
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	return h;
}

/**
* @brief Returns the windows aspect ratio
*
* @param window Window to get the ratio of
* @return The ratio of the given window
*/
float getRatio(GLFWwindow* window) {
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	return float(w) / float(h);
}