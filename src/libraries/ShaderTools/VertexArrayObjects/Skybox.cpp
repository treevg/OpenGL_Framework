#include "Skybox.h"

Skybox::Skybox() {
	mode = GL_TRIANGLE_STRIP;
    const float size = 1.0f;
    glGenVertexArrays(1, &vertexArrayObjectHandle);
    glBindVertexArray(vertexArrayObjectHandle);

    GLuint vertexBufferHandle;
    glGenBuffers(1, &vertexBufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
    float positions[] = {

 -10.0f, 10.0f, -10.0f,
-10.0f, -10.0f, -10.0f,
10.0f, -10.0f, -10.0f,
10.0f, -10.0f, -10.0f,
10.0f, 10.0f, -10.0f,
-10.0f, 10.0f, -10.0f,
-10.0f, -10.0f, 10.0f,
-10.0f, -10.0f, -10.0f,
-10.0f, 10.0f, -10.0f,
-10.0f, 10.0f, -10.0f,
-10.0f, 10.0f, 10.0f,
-10.0f, -10.0f, 10.0f,
10.0f, -10.0f, -10.0f,
10.0f, -10.0f, 10.0f,
10.0f, 10.0f, 10.0f,
10.0f, 10.0f, 10.0f,
10.0f, 10.0f, -10.0f,
10.0f, -10.0f, -10.0f,
-10.0f, -10.0f, 10.0f,
-10.0f, 10.0f, 10.0f,
10.0f, 10.0f, 10.0f,
10.0f, 10.0f, 10.0f,
10.0f, -10.0f, 10.0f,
-10.0f, -10.0f, 10.0f,
-10.0f, 10.0f, -10.0f,
10.0f, 10.0f, -10.0f,
10.0f, 10.0f, 10.0f,
10.0f, 10.0f, 10.0f,
-10.0f, 10.0f, 10.0f,
-10.0f, 10.0f, -10.0f,
-10.0f, -10.0f, -10.0f,
-10.0f, -10.0f, 10.0f,
10.0f, -10.0f, -10.0f,
10.0f, -10.0f, -10.0f,
-10.0f, -10.0f, 10.0f,
10.0f, -10.0f, 10.0f
    };
               
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), &positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);   

}

void Skybox::draw() {

  
    glBindVertexArray(vertexArrayObjectHandle);
 //   glDepthRange(1.0,1.0);
    glDrawArrays(mode, 0, 12*3);


}

