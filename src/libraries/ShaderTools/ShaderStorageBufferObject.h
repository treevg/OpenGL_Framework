#ifndef SHADER_STORAGE_BUFFER_OBJECT_H
#define SHADER_STORAGE_BUFFER_OBJECT_H

#include <GL/glew.h>

#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <fstream>
#include <glm/ext.hpp>
#include <map>

class ShaderStorageBufferObject {
public:
    ShaderStorageBufferObject();
    ShaderStorageBufferObject(int bindingPoint);
    ~ShaderStorageBufferObject();

    int bindingPoint;

    GLuint getHandle() const;

    template <typename T>
    void update(std::vector<T> data, GLenum usage )
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, handle);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(T) * data.size(), &data[0], usage);
        //glBindBufferRange(GL_SHADER_STORAGE_BUFFER, bindingPoint, handle, 0, sizeof(T) * data.size());
    }

    template <typename T>
    void update(std::vector<T> data)
    {
        this->update(data, mode);
    }

    void reset();

    void bindToShaderProgramBlock(GLuint shaderProgramHandle, std::string name);

private:
    GLuint handle;
    GLuint mode;
};

#endif // SHADER_STORAGE_BUFFER_OBJECT_H
