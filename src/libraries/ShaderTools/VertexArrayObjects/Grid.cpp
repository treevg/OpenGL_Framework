/*
 * Grid.cpp
 *
 *  Created on: 09.12.2014
 *      Author: Moe
 */

#include "Grid.h"

Grid::Grid(int width, int height) {
    mode = GL_POINTS;

    glGenVertexArrays(1, &vertexArrayObjectHandle);
    glBindVertexArray(vertexArrayObjectHandle);

    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    float positions[width*height];

    for(int i=0; i<height; i++){
    	for(int j=0; j<width; j++){
    		positions[height*i + j] = (i*(2/width))-1 + (j*(2/height))-1;
    	}
    }

    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    // ...(0,2,GL_FLOAT,...) originally
    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
}

void Grid::draw() {
    glBindVertexArray(vertexArrayObjectHandle);
    glDrawArrays(mode, 0, 1280*720);
}



