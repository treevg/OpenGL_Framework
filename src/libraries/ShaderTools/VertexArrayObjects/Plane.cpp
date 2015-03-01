#include <iostream>
#include <glm/glm.hpp> 
#include "Plane.h"

Plane::Plane() {
  mode = GL_TRIANGLE_STRIP;
   
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

    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

        GLuint positionTextureBuffer;

   GLfloat uvCoordinates[] = {
        
        0,1, 
        0,0,
        1,1,
        1,1,
        0,0,
        1.0
    };
    glBindBuffer(GL_ARRAY_BUFFER, positionTextureBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvCoordinates), uvCoordinates, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);




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