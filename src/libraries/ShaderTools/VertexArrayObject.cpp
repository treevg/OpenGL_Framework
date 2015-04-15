#include "VertexArrayObject.h"

VertexArrayObject* VertexArrayObject::setMode(GLenum mode) {
    this->mode = mode;
    return this;
}

  std::vector<MeshTexture> VertexArrayObject::getTextures() const{
   
          return this->m_textures;

    }