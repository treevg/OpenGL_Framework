// GLFWOculusRiftTest
// (c) cThrough 2014 (Daniel Dekkers)
// Version 2014120800 Based on DK2, OculusSDK 4.0.4

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
#include <../Include/OVR.h>
#include "../Src/Kernel/OVR_Math.h" 
#include <../Src/OVR_CAPI.h>
#include <../Src/OVR_CAPI_GL.h>
#include "glm\glm.hpp"
#include "ShaderTools/RenderPass.h"
#include "Compression/TextureTools.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"

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
static const bool DEVELOPMODE = TRUE;

GLfloat l_VAPoints[] =
{
	0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, -0.5f
};

GLfloat l_VANormals[] =
{
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f
};

GLuint l_VAIndici[] =
{
	0, 1, 2, 3,
	4, 5, 6, 7,
	8, 9, 10, 11,
	12, 13, 14, 15,
	16, 17, 18, 19,
	20, 21, 22, 23
};

// =============================================================================

static void ErrorCallback(int p_Error, const char* p_Description)
{
	fputs(p_Description, stderr);
}

// =============================================================================

static void KeyCallback(GLFWwindow* p_Window, int p_Key, int p_Scancode, int p_Action, int p_Mods)
{
	if (p_Action == GLFW_PRESS)
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
		}

		// Remove HSW on every key...
		ovrHSWDisplayState l_HasWarningState;
		ovrHmd_GetHSWDisplayState(g_Hmd, &l_HasWarningState);
		if (l_HasWarningState.Displayed) ovrHmd_DismissHSWDisplay(g_Hmd);
	}
}

// =============================================================================

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

// ============================================================================

void RenderCubeVertexArrays(void)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, l_VAPoints);

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, l_VANormals);

	glDrawElements(GL_QUADS, 6 * 4, GL_UNSIGNED_INT, l_VAIndici);

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

// ============================================================================

void RenderCubeFixedFunction(void)
{
	// Obsolete, remains as a fall back for the vertex arrays version...
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);

	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);

	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);

	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);

	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);

	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glEnd();
}

// ============================================================================

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

	// Material...
	GLfloat l_MaterialSpecular[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat l_MaterialShininess[] = { 10.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, l_MaterialSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, l_MaterialShininess);

	// Some (stationary) lights, position will be set every frame separately...
	GLfloat l_Light0Diffuse[] = { 1.0f, 0.8f, 0.6f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l_Light0Diffuse);
	glEnable(GL_LIGHT0);

	GLfloat l_Light1Diffuse[] = { 0.6f, 0.8f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, l_Light1Diffuse);
	glEnable(GL_LIGHT1);
}

// ============================================================================

static void SetStaticLightPositions(void)
{
	GLfloat l_Light0Position[] = { 3.0f, 4.0f, 2.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, l_Light0Position);

	GLfloat l_Light1Position[] = { -3.0f, -4.0f, 2.0f, 0.0f };
	glLightfv(GL_LIGHT1, GL_POSITION, l_Light1Position);
}

// =============================================================================

glm::vec2 getResolution(GLFWwindow* window) {
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	return glm::vec2(float(w), float(h));
}

glm::mat4 toGLM(OVR::Matrix4f mat)
{
	return glm::mat4(
		glm::vec4(mat.GetXBasis().x, mat.GetXBasis().y, mat.GetXBasis().z, 0.0f),
		glm::vec4(mat.GetYBasis().x, mat.GetYBasis().y, mat.GetYBasis().z, 0.0f),
		glm::vec4(mat.GetZBasis().x, mat.GetZBasis().y, mat.GetZBasis().z, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
		);
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

	// Create the FBO being a single one for both eyes (this is open for debate)...
	GLuint l_FBOId;
	glGenFramebuffers(1, &l_FBOId);
	glBindFramebuffer(GL_FRAMEBUFFER, l_FBOId);

	// The texture we're going to render to...
	GLuint l_TextureId;
	glGenTextures(1, &l_TextureId);
	// "Bind" the newly created texture : all future texture functions will modify this texture...
	glBindTexture(GL_TEXTURE_2D, l_TextureId);
	// Give an empty image to OpenGL (the last "0")
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, g_RenderTargetSize.w, g_RenderTargetSize.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	// Linear filtering...
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Create Depth Buffer...
	GLuint l_DepthBufferId;
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
	g_ProjectionMatrici[ovrEye_Left] = ovrMatrix4f_Projection(g_EyeRenderDesc[ovrEye_Left].Fov, 0.3f, 100.0f, true);
	g_ProjectionMatrici[ovrEye_Right] = ovrMatrix4f_Projection(g_EyeRenderDesc[ovrEye_Right].Fov, 0.3f, 100.0f, true);

	// IPD offset values will not change at runtime, we can set them here...
	g_EyeOffsets[ovrEye_Left] = g_EyeRenderDesc[ovrEye_Left].HmdToEyeViewOffset;
	g_EyeOffsets[ovrEye_Right] = g_EyeRenderDesc[ovrEye_Right].HmdToEyeViewOffset;

	// Initial camera position...
	g_CameraPosition.x = 0.0f;
	g_CameraPosition.y = 0.0f;
	g_CameraPosition.z = -2.0f;

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetWindowSizeCallback(window, WindowSizeCallback);

	GLfloat l_SpinX;
	GLfloat l_SpinY;

	// Do a single recenter to calibrate orientation to current state of the Rift...
	ovrHmd_RecenterPose(g_Hmd);


	//std::vector<std::string> depthShaders = { "/Test_Telepresence/fullscreen.vert", "/Test_Telepresence/simpleTexture.frag" };
	//Quad* quad = new Quad();
	//RenderPass* texturePass = new RenderPass(
	//	quad,
	//	new ShaderProgram(depthShaders));


	Cube* cube = new Cube(glm::vec3(0.0, 0.0, -1.0), 0.5f);
	std::vector<std::string> attachShaders = { "/Test_Telepresence/v.vert", "/Test_Telepresence/f.frag" };
	RenderPass* cubePass = new RenderPass(
		cube,
		new ShaderProgram(attachShaders));

	cubePass->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	// Main loop...
	unsigned int l_FrameIndex = 0;
	while (!glfwWindowShouldClose(window))
	{
		if (l_Spin)
		{
			l_SpinX = (GLfloat)fmod(glfwGetTime()*17.0, 360.0);
			l_SpinY = (GLfloat)fmod(glfwGetTime()*23.0, 360.0);
		}
		else
		{
			l_SpinX = 30.0f;
			l_SpinY = 40.0f;
		}

		// Begin the frame...
		ovrHmd_BeginFrame(g_Hmd, l_FrameIndex);

		// Get eye poses for both the left and the right eye. g_EyePoses contains all Rift information: orientation, positional tracking and
		// the IPD in the form of the input variable g_EyeOffsets.
		ovrHmd_GetEyePoses(g_Hmd, l_FrameIndex, g_EyeOffsets, g_EyePoses, NULL);

		// Bind the FBO...
		glBindFramebuffer(GL_FRAMEBUFFER, l_FBOId);

		// Clear...
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (int l_EyeIndex = 0; l_EyeIndex<ovrEye_Count; l_EyeIndex++)
		{
			ovrEyeType l_Eye = g_Hmd->EyeRenderOrder[l_EyeIndex];

			glViewport(
				g_EyeTextures[l_Eye].Header.RenderViewport.Pos.x,
				g_EyeTextures[l_Eye].Header.RenderViewport.Pos.y,
				g_EyeTextures[l_Eye].Header.RenderViewport.Size.w,
				g_EyeTextures[l_Eye].Header.RenderViewport.Size.h
				);

			// Pass projection matrix on to OpenGL...
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glMultMatrixf(&(g_ProjectionMatrici[l_Eye].Transposed().M[0][0]));

			// Create the model-view matrix and pass on to OpenGL...
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			// Multiply with orientation retrieved from sensor...
			OVR::Quatf l_Orientation = OVR::Quatf(g_EyePoses[l_Eye].Orientation);
			OVR::Matrix4f l_ModelViewMatrix = OVR::Matrix4f(l_Orientation.Inverted());
			glMultMatrixf(&(l_ModelViewMatrix.Transposed().M[0][0]));

			// Translation due to positional tracking (DK2) and IPD...
			glTranslatef(-g_EyePoses[l_Eye].Position.x, -g_EyePoses[l_Eye].Position.y, -g_EyePoses[l_Eye].Position.z);

			// Move the world forward a bit to show the scene in front of us...
			glTranslatef(g_CameraPosition.x, g_CameraPosition.y, g_CameraPosition.z);

			// (Re)set the light positions so they don't move along with the cube...
			SetStaticLightPositions();

			// Make the cube spin...
			glRotatef(l_SpinX, 1.0f, 0.0f, 0.0f);
			glRotatef(l_SpinY, 0.0f, 1.0f, 0.0f);

			// Render...
			// RenderCubeFixedFunction();
			//RenderCubeVertexArrays();
			
			glm::mat4 view = glm::mat4(1.0f);
			//glm::mat4 projection = glm::mat4(1.0f);
			glm::mat4 projection = glm::perspective(60.0f, (float)width / height, 0.1f, 1000.0f);

			//projection = toGLM(g_ProjectionMatrici[l_Eye].Transposed());
			glm::vec3 lightPos = glm::vec3(2, 2, 2);

			view = toGLM(l_ModelViewMatrix.Transposed());
			glm::vec3 trans = glm::vec3(-g_EyePoses[l_Eye].Position.x, -g_EyePoses[l_Eye].Position.y, -g_EyePoses[l_Eye].Position.z);
			view = glm::translate(view, trans );
			trans = glm::vec3(g_CameraPosition.x, g_CameraPosition.y, g_CameraPosition.z);
			view = glm::translate(view, trans);



			cubePass->update("lightPosition", lightPos)
				->update("modelMatrix", glm::mat4(1.0))
				->update("viewMatrix", view)
				->update("diffuseColor", glm::vec3(1.0, 1.0, 0.0))
				->update("projectionMatrix", projection)
				->run();

		}

		// Back to the default framebuffer...
		glBindFramebuffer(GL_FRAMEBUFFER, 0);





		// Do everything, distortion, front/back buffer swap...
		ovrHmd_EndFrame(g_Hmd, g_EyePoses, g_EyeTextures);

		//texturePass
		//	->update("resolution", getResolution(window));

		////texture settings
		//glGenerateMipmap(GL_TEXTURE_2D);
		//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, true);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


		//texturePass
		//	->clear(0.0, 0.0, 0.0, 1.0)
		//	->texture("tex", l_TextureId)
		//	//->texture("tex", TextureTools::loadTexture("C:/dev/Framework/resources/jpg/bambus.jpg"))
		//	->run();

		//glfwSwapBuffers(window);


		++l_FrameIndex;

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


