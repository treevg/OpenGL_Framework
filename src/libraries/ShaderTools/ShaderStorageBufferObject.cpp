#include "ShaderStorageBufferObject.h"


static int nextFreeBindingPoint = 0;

ShaderStorageBufferObject::ShaderStorageBufferObject()
{
    bindingPoint = nextFreeBindingPoint++;
    mode = GL_DYNAMIC_DRAW;
    glGenBuffers(1, &handle);
}

ShaderStorageBufferObject::ShaderStorageBufferObject(int bindingPoint)
{
    this->bindingPoint = bindingPoint;
    mode = GL_DYNAMIC_DRAW;
    glGenBuffers(1, &handle);
}

ShaderStorageBufferObject::~ShaderStorageBufferObject()
{

}

GLuint ShaderStorageBufferObject::getHandle() const
{
    return handle;
}

void ShaderStorageBufferObject::reset()
{
//    glBindBuffer(GL_SHADER_STORAGE_BUFFER, handle);
//    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(lastData[0]) * lastData.size(), &lastData[0], GL_DYNAMIC_READ);
    //    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, bindingPoint, handle, 0, sizeof(lastData[0]) * lastData.size());
}

void ShaderStorageBufferObject::bindToShaderProgramBlock(GLuint shaderProgramHandle, std::string name)
{
    GLuint block_index = 0;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, handle);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, handle);
    block_index = glGetProgramResourceIndex(shaderProgramHandle, GL_SHADER_STORAGE_BLOCK, name.c_str());
    glShaderStorageBlockBinding(shaderProgramHandle, block_index, bindingPoint);
}

