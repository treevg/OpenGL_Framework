/*
This testclass tests the implementation of the hierarchical hole filling for use with kinect data (rgb and depth-textures)
Used in research project FP14-Tele

created: bwolf 19.03.2015
*/
#include "ShaderTools/Renderer.h"
#include "AssetTools/Texture.h"
#include "ShaderTools/VertexArrayObjects/Grid.h"
#include "hhf.h"

using namespace std;
using namespace glm;

/*
Work in Progress.
Incomplete.
*/

// The window
GLFWwindow* window;

// Window width and height
int width = 1280;
int height = 720;

// Texture width and height
int texH, texW;

// maximum mipmap depth
int maxLevelSize;

int main(int argc, char *argv[])
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	//	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, "Test FP14 HHF", NULL, NULL);
	if (window == NULL){
		fprintf(stderr, "It seems like your GPU is not compatible with the demanded version of OpenGL. Please fix this.\n");
		glfwTerminate();
		return -1;
	}
	// make window current context
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Renderloop
	while (!glfwWindowShouldClose(window)){
		// do stuff

		// swap buffer
		glfwSwapBuffers(window);
		// poll events
		glfwPollEvents();
	}

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}