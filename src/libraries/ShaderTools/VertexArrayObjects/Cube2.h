#ifndef CUBE_2_H
#define CUBE_2_H

#include "../VertexArrayObject.h"
#define GLM_FORCE_RADIANS
#include <glm/ext.hpp>

using namespace glm;

class Cube2 : public VertexArrayObject {
public:
	Cube2(mat4 modelMatrix, vec4 color, GLuint textureHandle);
	void draw();
        
        
};

#endif // CUBE_H
