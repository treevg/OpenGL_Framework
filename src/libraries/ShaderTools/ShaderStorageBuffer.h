#ifndef SHADER_STORAGE_BUFFER_H
#define SHADER_STORAGE_BUFFER_H

#include "GL/glew.h"
#include <vector>
#include <glm/ext.hpp>
#include "Objectloader/Objectloader.h"

class ShaderStorageBuffer
{
public:
	//float meshData[];

	ShaderStorageBuffer();
	~ShaderStorageBuffer();

	ShaderStorageBuffer(const char * path);

	ShaderStorageBuffer* bind(GLuint binding);

protected:
	GLuint shaderStorageObjectHandle;
};

#endif // SHADER_STORAGE_BUFFER_H
