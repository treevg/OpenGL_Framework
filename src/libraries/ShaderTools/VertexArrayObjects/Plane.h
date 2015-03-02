#ifndef PLANE_H
#define PLANE_H

#include <vector>
#include "../VertexArrayObject.h"

class Plane : public VertexArrayObject {
public:
	Plane(float);
	void draw();
};

#endif // PLANE_H
