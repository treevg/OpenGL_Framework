#ifndef DEFAULT_RENDER_LOOP_H
#define DEFAULT_RENDER_LOOP_H

#include "iostream"
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

static const int width = 1280;
static const int height = 720;
static GLFWwindow *window;

class DefaultRenderLoop
{
public:
	DefaultRenderLoop() {
	    glfwInit();

	    window = glfwCreateWindow(width, height, "OpenGL Window", NULL, NULL);
	    glfwSetWindowPos( window, 0, 100);
	    glfwMakeContextCurrent(window);

	    glewInit();
	}

	void start(void (*renderLoop)()) {
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);

		while ( !glfwWindowShouldClose(window)) {
	        renderLoop();

	        glfwSwapBuffers(window);
	        glfwPollEvents();
	    }

	    glfwDestroyWindow(window);
	    glfwTerminate();
	};
};

static auto defaultRenderLoop = new DefaultRenderLoop();
static void renderLoop(void (*renderLoop)()) {
	defaultRenderLoop->start(renderLoop);
}

#endif // DEFAULT_RENDER_LOOP_H
