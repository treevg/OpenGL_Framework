#include "VertexArrayObject.h"

VertexArrayObject* VertexArrayObject::setMode(GLenum mode) {
    this->mode = mode;
    return this;
}

