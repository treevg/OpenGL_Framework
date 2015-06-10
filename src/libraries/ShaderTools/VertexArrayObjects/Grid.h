/*
 * Grid.h
 *
 *  Created on: 09.12.2014
 *      Author: Moe
 */

#ifndef LIBRARIES_SHADERTOOLS_VERTEXARRAYOBJECTS_GRID_H_
#define LIBRARIES_SHADERTOOLS_VERTEXARRAYOBJECTS_GRID_H_



#include "../VertexArrayObject.h"

class Grid : public VertexArrayObject {
public:
	int count;
	Grid(int width, int height);
    void draw();
};




#endif /* LIBRARIES_SHADERTOOLS_VERTEXARRAYOBJECTS_GRID_H_ */
