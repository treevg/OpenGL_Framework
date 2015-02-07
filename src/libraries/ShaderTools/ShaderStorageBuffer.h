#ifndef SHADER_STORAGE_BUFFER_H
#define SHADER_STORAGE_BUFFER_H

#include "GL/glew.h"

class ShaderStorageBuffer
{
public:
	ShaderStorageBuffer();
	~ShaderStorageBuffer();

	ShaderStorageBuffer* bind(GLuint binding);

protected:
	GLuint shaderStorageObjectHandle;
};

#endif // SHADER_STORAGE_BUFFER_H
