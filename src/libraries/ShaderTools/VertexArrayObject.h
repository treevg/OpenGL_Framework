#ifndef VERTEX_ARRAY_OBJECT_H
#define VERTEX_ARRAY_OBJECT_H

#include <GL/glew.h>

#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <fstream>
#define GLM_FORCE_RADIANS
#include <glm/ext.hpp>
#include <map>

class VertexArrayObject {
public:
	VertexArrayObject* setMode(GLenum mode);
	virtual void draw() = 0;
protected:
	GLuint vertexArrayObjectHandle;
	GLenum mode;
};

#endif // VERTEX_ARRAY_OBJECT_H
