/**
 * @file   		Renderer.h
 * @author 		Gerrit Lochmann
 * @date   		@todo
 * @copyright	@todo
 *
 * @brief  		Default renderer
 *
 * The Renderer class creates a default renderer by providing window creation, 
 * keyboard callbacks and a default render loop function.
 */
#ifndef DEFAULT_RENDER_LOOP_H
#define DEFAULT_RENDER_LOOP_H

#include <functional>
#include "iostream"
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

//
// @brief Erzeugt ein Fenster
// @details [long description]
// 
// @param width		Breite des Fensters
// @param height	Höhe des Fenster
// @param posX 		Position des Fensters in X-Richtung
// @param posY 		Position des Fensters in Y-Richtung
// @return 			Pointer auf das Fenster
// 

GLFWwindow* generateWindow(int width = 1280, int height = 720, int posX = 100, int posY = 100) {
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Window", NULL, NULL);
	glfwSetWindowPos(window, posX, posY);
	glfwSetWindowSize(window, width, height);
	glfwMakeContextCurrent(window);

	glewInit();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);	

	return window;
}

// @brief Default Render-Loop
// @details Der Default Render-Loop wartet darauf, dass das Fenster geschlossen
// 			wird löscht es dann und terminiert GLFW. Außerdem wird schon ein
// 			Delta Zeiwert berechnet und die Framebuffer gtauscht (SwapBuffers)
// 
// @param window 	Fenster in das gerendert werden soll
// @param loop 		Function pointer zur Function, die im Render-Loop ausgeführt
// 					werden soll

void render(GLFWwindow* window, std::function<void (double)> loop) {
	float lastTime = 0.0;
	while ( !glfwWindowShouldClose(window)) {
		float currentTime = glfwGetTime();
		loop(currentTime - lastTime);
		lastTime = currentTime;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
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

void setKeyCallback(GLFWwindow* window, std::function<void (int, int, int, int)> func) {
	static std::function<void (int, int, int, int)> func_bounce = func;
	glfwSetKeyCallback(window, [] (GLFWwindow* w, int k, int s, int a, int m) {
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
void setMouseButtonCallback(GLFWwindow* window, std::function<void (int, int, int)> func) {
	static std::function<void (int, int, int)> func_bounce = func;
	glfwSetMouseButtonCallback(window, [] (GLFWwindow* w, int b, int a, int m) {
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
void setCharCallback(GLFWwindow* window, std::function<void (unsigned int)> func) {
	static std::function<void (unsigned int)> func_bounce = func;
	glfwSetCharCallback(window, [] (GLFWwindow* w, unsigned int c) {
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
void setCursorPosCallback(GLFWwindow* window, std::function<void (double, double)> func) {
	static std::function<void (double, double)> func_bounce = func;
	glfwSetCursorPosCallback(window, [] (GLFWwindow* w, double x, double y) {
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
void setScrollCallback(GLFWwindow* window, std::function<void (double, double)> func) {
	static std::function<void (double, double)> func_bounce = func;
	glfwSetScrollCallback(window, [] (GLFWwindow* w, double x, double y) {
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
void setCursorEnterCallback(GLFWwindow* window, std::function<void (int)> func) {
	static std::function<void (int)> func_bounce = func;
	glfwSetCursorEnterCallback(window, [] (GLFWwindow* w, int e) {
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
glm::vec2 getResolution(GLFWwindow* window) {
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    return glm::vec2(float(w), float(h));
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
    return float(w)/float(h);
}


#endif // DEFAULT_RENDER_LOOP_H
