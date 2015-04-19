#include "Skybox2.h"
//USE WITH 
//attribute-less rendering 
Skybox2::Skybox2() {
    mode = GL_TRIANGLE_STRIP;
    
    glGenVertexArrays(1, &vertexArrayObjectHandle);
    glBindVertexArray(vertexArrayObjectHandle);
 

}

void Skybox2::draw() {

  
    glBindVertexArray(vertexArrayObjectHandle);
 
    glDrawArrays(mode, 0, 4);
    


}

