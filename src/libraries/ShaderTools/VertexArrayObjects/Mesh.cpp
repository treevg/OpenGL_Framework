#include "Mesh.h"


   Mesh::Mesh(vector<vec3> verticies, vector<vec3> normals, vector<vec2> textCoord,  vector<GLuint> indicies){
  
     mode = GL_TRIANGLE_STRIP;

   this->m_verticies = verticies;
   this->m_normals = normals;
   this->m_texCoords = textCoord;
   this->m_indicies = indicies;

    glGenVertexArrays(1, &vertexArrayObjectHandle);
    glBindVertexArray(vertexArrayObjectHandle);

    GLuint vertexBufferHandles[4];

    glGenBuffers(4, vertexBufferHandles);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[0]);     
    //better index array        
    glBufferData(GL_ARRAY_BUFFER, this->m_verticies.size() * sizeof(vec3), &this->m_verticies[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(0);
   //for indicies

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[1]);
    glBufferData(GL_ARRAY_BUFFER, this->m_indicies.size()*sizeof(GLuint), &this->m_indicies[0], GL_STATIC_DRAW);
   // glEnableVertexAttribArray(1);

     //for Texture coordinates

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[2]);
    glBufferData(GL_ARRAY_BUFFER, this->m_texCoords.size()*sizeof(vec2), &this->m_texCoords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);


//for normals
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[3]);
    glBufferData(GL_ARRAY_BUFFER, this->m_normals.size()*sizeof(vec3), &this->m_normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);
     
   }



vector<vec3> Mesh::getVerticies() const{
    
     return this->m_verticies;

}
  vector<vec3> Mesh::getNormals() const{
        
        return this->m_normals;

  }
  vector<vec2> Mesh::getTexCoords() const{

        return this->m_texCoords;
  }

   
    vector<GLuint> Mesh::Mesh::getIndicies() const{

          return  this->m_indicies;

    }
    vector<Texture> Mesh::getTextures() const{
   
          return this->m_textures;

    }
    


//calculate vericies number for verticies
void Mesh::draw() {
    glBindVertexArray(vertexArrayObjectHandle);
  //  glDrawArrays(mode, 0, this->numberOfIndicies);
    glDrawElements(mode, this->m_indicies.size(), GL_UNSIGNED_INT, 0);
    //set to default for good praxice 
    glBindVertexArray(0);
}



