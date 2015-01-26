/*
 * Grid.cpp
 *
 *  Created on: 09.12.2014
 *      Author: Moe
 */

#include "Grid.h"

Grid::Grid(int w, int h) {
    mode = GL_POINTS;
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH,GL_NICEST);

    glGenVertexArrays(1, &vertexArrayObjectHandle);
    glBindVertexArray(vertexArrayObjectHandle);

    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);

    //float positions[width*height*2];
    float *float_array = new float[w * h * 2];
    count = 0;
    for(int y = 0; y < h; y++){
    	for(int x = 0; x < w; x++){
    		float_array[count++] = (float) x / ((float) w + 0.5);
    		float_array[count++] = (float) y / ((float) h + 0.5);
            // std::cout << float_array[count - 1] << " " << float_array[count - 2] << "\n";
    		// std::cout<<"GRRR1 "<<width*i+j<<"::"<<float_array[width*i+j]<<"::"<<float_array[width*i+j+1]<<std::endl;
    	}
    }
    std::cout << w << " " << h << " " << count << " " << w * h * 2 << std::endl;
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * count, float_array, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
}

void Grid::draw() {
    glBindVertexArray(vertexArrayObjectHandle);
    glDrawArrays(mode, 0, count);
}



