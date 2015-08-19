#ifndef ROOM_H
#define ROOM_H

#include "../../libraries/ShaderTools/VertexArrayObject.h"

class Room : public VertexArrayObject {
public:
	Room();
	void draw();

private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> u;
	std::vector<glm::vec3> n;

};

#endif // ROOM_H