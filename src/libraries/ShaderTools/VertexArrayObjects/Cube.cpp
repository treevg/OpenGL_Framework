#include "Cube.h"

Cube::Cube() {
	const float size = 1.0f;
	mode = GL_TRIANGLE_STRIP;

    glGenVertexArrays(1, &vertexArrayObjectHandle);
    glBindVertexArray(vertexArrayObjectHandle);

    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
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
}

void Cube::draw() {
    glBindVertexArray(vertexArrayObjectHandle);
    glDrawArrays(mode, 0, 12*3);
}
