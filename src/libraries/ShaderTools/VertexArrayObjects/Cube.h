#ifndef CUBE_H
#define CUBE_H

#include "../VertexArrayObject.h"

class Cube : public VertexArrayObject {
public:
	Cube();
	Cube(glm::vec3 position, float size);
	void draw();
	std::vector<glm::vec3> getVertices();

private:
	std::vector<glm::vec3> vertices;
};

#endif // CUBE_H
