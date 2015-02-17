#include "Mesh.h"


//just for debugin
 void Mesh::printInd(){
  cout<< "printing indexes" << endl;
  cout<< "size " << this->m_indices.size()  << endl;
  
for ( int i = 0; i< this->m_indices.size(); i++){
    cout<< "index " << this->m_indices[i] << endl; 
}

}


   Mesh::Mesh(vector<vec3> vertices, vector<vec3> normals, vector<vec2> textCoord,  vector<GLuint> indices){
    

   this->m_vertices = vertices;
   this->m_normals = normals;
   this->m_texCoords = textCoord;
   this->m_indices = indices;

   cout<< "size indices " << this->m_indices.size()  << endl;
    cout<< "size  vertices " << this->m_vertices.size()  << endl;
    cout<< "size  normals " << this->m_normals.size()  << endl;
     cout<< "size  textCoord " << this->m_texCoords.size()  << endl;
     
    glGenVertexArrays(1, &vertexArrayObjectHandle);
    glBindVertexArray(vertexArrayObjectHandle);

    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);  
  
   
    // for Positions
    glBufferData(GL_ARRAY_BUFFER, this->m_vertices.size() * sizeof(vec3), &this->m_vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(0);
  
   //for indices
    glGenBuffers(1, &this->IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
    cout << "IBO " << this->IBO << endl;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_indices.size()*sizeof(GLuint), &this->m_indices[0], GL_STATIC_DRAW);
  


     // printInd();

     //for Texture coordinates
    glGenBuffers(1, &this->tVBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->tVBO);
    cout << "tVBO " << this->tVBO << endl;
    glBufferData(GL_ARRAY_BUFFER, this->m_texCoords.size()*sizeof(vec2), &this->m_texCoords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);


 //for Normals
  /*  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[3]);
    glBufferData(GL_ARRAY_BUFFER, this->m_normals.size()*sizeof(vec3), &this->m_normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2); */
     
   }



vector<vec3> Mesh::getVertices() const{
    
     return this->m_vertices;

}
  vector<vec3> Mesh::getNormals() const{
        
        return this->m_normals;

  }
  vector<vec2> Mesh::getTexCoords() const{

        return this->m_texCoords;
  }

   
    vector<GLuint> Mesh::Mesh::getIndices() const{

          return  this->m_indices;

    }
    vector<Texture> Mesh::getTextures() const{
   
          return this->m_textures;

    }
    


//calculate vericies number for verticies
void Mesh::draw() {
   // cout << "drawing mesh" << endl;
    glBindVertexArray(vertexArrayObjectHandle);
    glDrawElements(GL_TRIANGLES, this->m_indices.size(), GL_UNSIGNED_INT, 0);
    //set to default for good praxice 
    glBindVertexArray(0);
}



