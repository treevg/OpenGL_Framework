#include <iostream>
#include <glm/glm.hpp> 
#include "Plane.h"

Plane::Plane() {
  mode = GL_TRIANGLE_STRIP;
   // mode = GL_LINE_LOOP;
    glLineWidth(3);
    glGenVertexArrays(1, &vertexArrayObjectHandle);
    glBindVertexArray(vertexArrayObjectHandle);

    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    float positions[] = {
     -50.0f, 0.0f, -50.0f,
     -50.0f, 0.0f, 50.0f,
      50.0f, 0.0f, -50.0f,
      50.0f, 0.0f, -50.0f,
     -50.0f, 0.0f, 50.0f,
      50.0f, 0.0f, 50.0f 
    };

    //actually it had to be a grid :) 
    //how  does line_loop look like??? 

    std::vector<glm::vec3> verticies; 

    for (float z=-50.0f; z < 51.0f; z=z+1.0f){
    
      for (float x=-50.0f; x < 51.0f; x=x+1.0f){
        
          glm::vec3 vertex = glm::vec3(x,0.0f,z);

           verticies.push_back(vertex);
      
    }


    }

    //create vector with indexen

  


    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
}

void Plane::draw() {

  /*glDrawElements(
     GL_TRIANGLES,      // mode
     indices.size(),    // count
     GL_UNSIGNED_INT,   // type
     (void*)0           // element array buffer offset
 ); */

    glBindVertexArray(vertexArrayObjectHandle);
    glDrawArrays(mode, 0, 6);
}