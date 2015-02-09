#include "GenericVertexArrayObject.h"

GenericVertexArrayObject::GenericVertexArrayObject() {
    const float size = 0.5f;
    mode = GL_TRIANGLES;
    triangleCount = 12;

    glGenVertexArrays(1, &vertexArrayObjectHandle);
    glBindVertexArray(vertexArrayObjectHandle);

    // Vertices
    GLuint vertexBufferHandles[2];
    glGenBuffers(2, vertexBufferHandles);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[0]);
    float positions[] = {
                -size,-size,size, size,-size,size, size,size,size,
                size,size,size, -size,size,size, -size,-size,size,
                // Right face
                size,-size,size, size,-size,-size, size,size,-size,
                size,size,-size, size,size,size, size,-size,size,
                // Back face
                -size,-size,-size, size,-size,-size, size,size,-size,
                size,size,-size, -size,size,-size, -size,-size,-size,
                // Left face
                -size,-size,size, -size,-size,-size, -size,size,-size,
                -size,size,-size, -size,size,size, -size,-size,size,
                // Bottom face
                -size,-size,size, size,-size,size, size,-size,-size,
                size,-size,-size, -size,-size,-size, -size,-size,size,
                // Top Face
                -size,size,size, size,size,size, size,size,-size,
                size,size,-size, -size,size,-size, -size,size,size,
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Indices
    // @todo

    // Normals
    // @todo

    // UVS
    GLfloat uvCoordinates[] = {
        // Front face
        0,0, 1,0, 1,1,
        1,1, 0,1, 0,0,
        // Right face
        0,0, 1,0, 1,1,
        1,1, 0,1, 0,0,
        // Back face
        0,0, 1,0, 1,1,
        1,1, 0,1, 0,0,
        // Left face
        0,0, 1,0, 1,1,
        1,1, 0,1, 0,0,
        // Bottom face
        0,0, 1,0, 1,1,
        1,1, 0,1, 0,0,
        // Top face
        0,0, 1,0, 1,1,
        1,1, 0,1, 0,0,
    };
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvCoordinates), uvCoordinates, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
}

void GenericVertexArrayObject::draw() {
    glBindVertexArray(vertexArrayObjectHandle);
    glDrawArrays(mode, 0, triangleCount*3);
}
