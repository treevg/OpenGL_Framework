#ifndef DEFAULT_RENDER_LOOP_H
#define DEFAULT_RENDER_LOOP_H

#include "iostream"
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class DefaultRenderLoop
{
public:
	DefaultRenderLoop() {
	    glfwInit();

	    window = glfwCreateWindow(width, height, "OpenGL Window", NULL, NULL);
	    glfwSetWindowPos( window, 0, 100);
	    glfwMakeContextCurrent(window);

	    glewInit();
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	}

	const int width = 1280;
	const int height = 720;
	GLFWwindow *window;

	void start(void (*renderLoop)()) {
	    glEnable(GL_TEXTURE_2D);

		while ( !glfwWindowShouldClose(window)) {
	        glClear(GL_COLOR_BUFFER_BIT);

	        renderLoop();

	        glfwSwapBuffers(window);
	        glfwPollEvents();
	    }

	    glfwDestroyWindow(window);
	    glfwTerminate();
	};
};

static auto defaultRenderLoop = new DefaultRenderLoop();
static void start(void (*renderLoop)()) {
	defaultRenderLoop->start(renderLoop);
}

#endif // DEFAULT_RENDER_LOOP_H