#include "Model.h"

Model::Model(vector<float>positions, vector<unsigned int> textCoordinates) {
	mode = GL_TRIANGLE_STRIP;

    int sizeOfPositions = positions.size();
    cout<< " Positions: " << sizeOfPositions << endl;

    int sizeOfTexturePositions= textCoordinates.size();

     cout<< " TextPositions: " << sizeOfTexturePositions << endl;

    
    glGenVertexArrays(1, &vertexArrayObjectHandle);
    glBindVertexArray(vertexArrayObjectHandle);

    GLuint vertexBufferHandles[2];
    glGenBuffers(2, vertexBufferHandles);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[0]);             
    glBufferData(GL_ARRAY_BUFFER, sizeOfPositions*sizeof(float) , &positions, GL_STATIC_DRAW);
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
    glBufferData(GL_ARRAY_BUFFER, sizeOfTexturePositions*sizeof(int), &textCoordinates, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
}


//calculate vericies number for verticies
void Model::draw() {
    glBindVertexArray(vertexArrayObjectHandle);
    glDrawArrays(mode, 0, 12*3);
}

