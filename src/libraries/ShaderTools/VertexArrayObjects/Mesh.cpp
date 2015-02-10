#include "Mesh.h"


   Mesh::Mesh(vector<Vertex> verticies, vector<GLuint> indicies, vector<Texture> textures){
  
     mode = GL_TRIANGLE_FAN;

   this->verticies = verticies;
   this->indicies = indicies;
   this->textures = textures;

    glGenVertexArrays(1, &vertexArrayObjectHandle);
    glBindVertexArray(vertexArrayObjectHandle);

    GLuint vertexBufferHandles[3];

    glGenBuffers(3, vertexBufferHandles);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[0]);     
    //better index array        
    glBufferData(GL_ARRAY_BUFFER, this->verticies.size() * sizeof(Vertex), &this->verticies[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);


   //for indecies

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[1]);
    glBufferData(GL_ARRAY_BUFFER, this->indicies.size()*sizeof(GLuint), &this->indicies[0], GL_STATIC_DRAW);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)offsetof(Vertex, Normal);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);


     //for Texture coordinates

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[2]);
    glBufferData(GL_ARRAY_BUFFER, this->textures.size()*sizeof(Texture), &this->textures[0], GL_STATIC_DRAW);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)offsetof(Vertex, Normal);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);
   
      glBindVertexArray(0);


   }




    vector<Vertex> Mesh::getVerticies() const{

     return this->verticies;


    }
    vector<GLuint> Mesh::getIndicies() const{

      return  this->indicies;

    }
    vector<Texture> Mesh::getTextures() const{

     return this->textures;

    }
    





//calculate vericies number for verticies
void Mesh::draw() {
    glBindVertexArray(vertexArrayObjectHandle);
  //  glDrawArrays(mode, 0, this->numberOfIndicies);
    glDrawElements(mode, this->indicies.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}



