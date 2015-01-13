#ifndef OPENGLTESTUTILITIES_H_
#define OPENGLTESTUTILITIES_H_

#include "ShaderTools/DefaultRenderLoop.h"

static void testZeros(GLuint texture, GLenum format = GL_RGBA);
static void testZeros1D(GLuint texture);

static void testError();
static void testFramebuffer(GLenum target);

static void printImageBindings();

static void startTime();
static void stopTime();
#endif
