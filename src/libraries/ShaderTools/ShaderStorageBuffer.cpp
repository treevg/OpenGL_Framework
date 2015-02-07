#include "ShaderStorageBuffer.h"

ShaderStorageBuffer::ShaderStorageBuffer() {
    glGenBuffers(1, &shaderStorageObjectHandle);

    float size = 1;
    float meshData[] = {
                -size,-size,size,0, size,-size,size,0, size,size,size,0,
                size,size,size,0, -size,size,size,0, -size,-size,size,0,
                // Right face
                size,-size,size,0, size,-size,-size,0, size,size,-size,0,
                size,size,-size,0, size,size,size,0, size,-size,size,0,
                // Back face
                -size,-size,-size,0, size,-size,-size,0, size,size,-size,0,
                size,size,-size,0, -size,size,-size,0, -size,-size,-size,0,
                // Left face
                -size,-size,size,0, -size,-size,-size,0, -size,size,-size,0,
                -size,size,-size,0, -size,size,size,0, -size,-size,size,0,
                // Bottom face
                -size,-size,size,0, size,-size,size,0, size,-size,-size,0,
                size,-size,-size,0, -size,-size,-size,0, -size,-size,size,0,
                // // Top Face
                -size,size,size,0, size,size,size,0, size,size,-size,0,
                size,size,-size,0, -size,size,-size,0, -size,size,size,0,

   };

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, shaderStorageObjectHandle);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(meshData), meshData, GL_STATIC_DRAW);
        // glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        // glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);

    // GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
    // memcpy(p, &meshData, sizeof(meshData));
    // glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    // GLuint ssbi = 7;
    // ssbi = glGetProgramResourceIndex(sp->getProgramHandle(), GL_SHADER_STORAGE_BLOCK, "meshData");
    // GLuint ssbo_binding_point_index = 15;
    // glShaderStorageBlockBinding(sp->getProgramHandle(),ssbi, ssbo_binding_point_index );
    // cout<<"  SSBI  "<<ssbi<< endl;
}

ShaderStorageBuffer* ShaderStorageBuffer::bind(GLuint binding) {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, shaderStorageObjectHandle);  
    // glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
