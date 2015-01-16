#include "Pyramid.h"

Pyramid::Pyramid() {
    mode =  GL_TRIANGLES;

    glGenVertexArrays(1, &vertexArrayObjectHandle);
    glBindVertexArray(vertexArrayObjectHandle);

    GLuint vertexBufferHandles[2];
    glGenBuffers(2, vertexBufferHandles);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[0]);

 /*   -0.3f, -0.5f,  0.3f,    / 0 front left bottom 
     0.3f, -0.5f,  0.3f,    /  1 front right bottom
     0.3f, -0.5f, -0.3f,     / 2 back right bottom
    -0.3f, -0.5f, -0.3f,     / 3 back left bottom
     0.0f,  0.5f,  0.0f,     // 4 apex   */
  
    float positions[] = {
     //front face
      -0.3f, -0.5f,  0.3f, 
      0.3f, -0.5f,  0.3f, 
      0.0f,  0.5f,  0.0f, 

      //right face

      0.3f, -0.5f,  0.3f,    
      0.3f, -0.5f, -0.3f, 
       0.0f,  0.5f,  0.0f,

       //back face 

       0.3f, -0.5f, -0.3f,     
      -0.3f, -0.5f, -0.3f, 
       0.0f,  0.5f,  0.0f,

       //left face
    
      -0.3f, -0.5f, -0.3f,
      -0.3f, -0.5f,  0.3f, 
      0.0f,  0.5f,  0.0f, 

      //bottom face
 
     -0.3f, -0.5f, -0.3f,
     0.3f, -0.5f, -0.3f,
      0.3f, -0.5f,  0.3f,


      -0.3f, -0.5f, -0.3f,
       -0.3f, -0.5f, -0.3f,
       0.3f, -0.5f, -0.3f, };


                 
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);


 //change this for pyramid
    GLfloat uvCoordinates[] = {
        // Front face
        0,0, 1,0, 1,1,
        1,1, 0,1, 0,0,
        // Right face
        0,0, 1,0, 1,1,
        1,1, 0,1, 0,0,
        // Back face
        0,0, 1,0, 1,1,
        1,1, 0,1, 0,0,
        // Left face
        0,0, 1,0, 1,1,
        1,1, 0,1, 0,0,
        // Bottom face
        0,0, 1,0, 1,1,
        1,1, 0,1, 0,0,
        // Top face
        0,0, 1,0, 1,1,
        1,1, 0,1, 0,0,
    };
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvCoordinates), uvCoordinates, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
}

void Pyramid::draw() {
    glBindVertexArray(vertexArrayObjectHandle);
    glDrawArrays(mode, 0, 18);
}

