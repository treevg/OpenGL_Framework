#include "GenericVertexArrayObject.h"

GenericVertexArrayObject::GenericVertexArrayObject(GLuint vao) {
   vertexArrayObjectHandle = vao;
}

void GenericVertexArrayObject::draw() {
    glBindVertexArray(vertexArrayObjectHandle);
    glDrawArrays(mode, 0, triangleCount*3);
}
