#ifndef SKYBOX_H
#define SKYBOX_H

#include "../VertexArrayObject.h"

class Skybox: public VertexArrayObject {
public:
	Skybox();
	void draw();
};

#endif // SKYBOX_H