#include "Cube2.h"

Cube2::Cube2(mat4 modelMatrix, GLuint textureHandle, string shaderProgramKey) {

	const float size = 1.0f;
	mode = GL_TRIANGLES;
    this->color = color;
    this->modelMatrix = modelMatrix;
    this->textureHandle = textureHandle;
    this->shaderProgramKey = shaderProgramKey;

    glGenVertexArrays(1, &vertexArrayObjectHandle);
    glBindVertexArray(vertexArrayObjectHandle);

    GLuint vertexBufferHandles[2];
    glGenBuffers(2, vertexBufferHandles);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[0]);
    float positions[] = {
		        -size,-size,size, size,-size,size, size,size,size,
		        size,size,size, -size,size,size, -size,-size,size,
		        // Right face
		        size,-size,size, size,-size,-size, size,size,-size,
		        size,size,-size, size,size,size, size,-size,size,
		        // Back face
		        -size,-size,-size, size,-size,-size, size,size,-size,
		        size,size,-size, -size,size,-size, -size,-size,-size,
		        // Left face
		        -size,-size,size, -size,-size,-size, -size,size,-size,
		        -size,size,-size, -size,size,size, -size,-size,size,
		        // Bottom face
		        -size,-size,size, size,-size,size, size,-size,-size,
		        size,-size,-size, -size,-size,-size, -size,-size,size,
		        // Top Face
		        -size,size,size, size,size,size, size,size,-size,
		        size,size,-size, -size,size,-size, -size,size,size,
    };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

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

void Cube2::draw() {
    glBindVertexArray(vertexArrayObjectHandle);
    glDrawArrays(mode, 0, 12*3);
}
