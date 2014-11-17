/*
 * ComputeShaderTools.cpp
 *
 *  Created on: 10.11.2014
 *      Author: Batsi
 */

#include <GL/glew.h>

#include "ComputeShaderTools.h"

namespace ComputeShaderTools{

GLuint generateTexture(){
	GLuint texHandle;
	glGenTextures(1, &texHandle);

	glBindTexture(GL_TEXTURE_2D, texHandle);

	// Because we're also using this tex as an image (in order to write to it),
	// we bind it to an image unit as well
	glBindImageTexture(1, texHandle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	return texHandle;
}

}
