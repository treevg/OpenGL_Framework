#include "Quad.h"

Quad::Quad() {
    mode = GL_TRIANGLE_STRIP;

    glGenVertexArrays(1, &vertexArrayObjectHandle);
    glBindVertexArray(vertexArrayObjectHandle);

    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    float positions[] = {
        -1.0f, -1.0f,
        -1.0f, 1.0f,
        1.0f, -1.0f,
        1.0f, 1.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8, positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
}

void Quad::draw() {
    glBindVertexArray(vertexArrayObjectHandle);
    glDrawArrays(mode, 0, 4);
}

void Quad::drawInstanced(int countInstances) {
    glBindVertexArray(vertexArrayObjectHandle);
    glDrawArraysInstanced(mode, 0, 4, countInstances);
}

void Quad::prepareInstancedDrawing() {
    /* testweise implementierung ... */

}


