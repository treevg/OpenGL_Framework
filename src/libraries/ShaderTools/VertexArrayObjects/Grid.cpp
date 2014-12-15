/*
 * Grid.cpp
 *
 *  Created on: 09.12.2014
 *      Author: Moe
 */

#include "Grid.h"

Grid::Grid(int w, int h) {
    mode = GL_POINTS;
    width=50;
    height=50;

    glGenVertexArrays(1, &vertexArrayObjectHandle);
    glBindVertexArray(vertexArrayObjectHandle);

    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);

    //float positions[width*height*2];
   float *float_array = new float[width*height*2];

    for(int i=0; i<height; i++){
    	for(int j=0; j<2*width; j+=2){
    		float_array[width*i + j] = (float) ((j/2)*(2.0/width))-1;
    		float_array[width*i + j+1] = (float) (i*(2.0/height))-1;
    		std::cout<<"GRRR1 "<<width*i+j<<"::"<<float_array[width*i+j]<<"::"<<float_array[width*i+j+1]<<std::endl;

    	}
    }

    glBufferData(GL_ARRAY_BUFFER, sizeof(*float_array), float_array, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
}

void Grid::draw() {
    glBindVertexArray(vertexArrayObjectHandle);
    glDrawArrays(mode, 0, width*height);
}



