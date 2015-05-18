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
 

 mat4 VertexArrayObject::getModelMatrix() const{

   return this->modelMatrix;
 }


   vector<float> VertexArrayObject::getSacalarVertices() const{

     vector<float> scalarVerticies;

     for(vec3 vertex: this->m_vertices){

        scalarVerticies.push_back(vertex.x);
        scalarVerticies.push_back(vertex.y);
        scalarVerticies.push_back(vertex.z);

     }

     return scalarVerticies;

   }