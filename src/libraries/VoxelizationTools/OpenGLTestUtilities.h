#ifndef OPENGLTESTUTILITIES_H_
#define OPENGLTESTUTILITIES_H_

#include "iostream"
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/** prints output on console if texture contains only zeros or prints first value that is not zero */
void testZeros(GLuint texture, GLenum format = GL_RGBA, GLenum target = GL_TEXTURE_2D );
/** prints output on console if 1D-texture contains only zeros or prints first value that is not zero */
void testZeros1D(GLuint texture);

/** queries current OpenGL Error Status and prints output */
void testError();
/** queries current OpenGL Error Status (of currently bound framebuffer) for a Framebuffer target and prints output */
void testFramebuffer(GLenum target);

/** prints texture names that are currently bound to image units */
void printImageBindings();

/** produces a time query object for the current GPU time */
void startTime();
/** prints the time that has passed since startTime() was called. WARNING: effectively stalls thread by looping until GPU response is received */
void stopTime();

#endif
