#include "VertexArrayObject.h"

VertexArrayObject* VertexArrayObject::setMode(GLenum mode) {
    this->mode = mode;
    return this;
}

  std::vector<MeshTexture> VertexArrayObject::getTextures() const{
   
          return this->m_textures;

    }


    Material VertexArrayObject::getMaterial() const{

    	return this->material;
    }



     void VertexArrayObject::setModelMatrix(mat4 matrix){

     	this->modelMatrix=matrix;
     }



     vector<vec3> VertexArrayObject::getVertices() const{
    
     return this->m_vertices;

}
  vector<vec3> VertexArrayObject::getNormals() const{
        
        return this->m_normals;

  }
  vector<vec2> VertexArrayObject::getTexCoords() const{

        return this->m_texCoords;
  }

   
    vector<GLuint> VertexArrayObject::getIndices() const{

          return  this->m_indices;

    }
 