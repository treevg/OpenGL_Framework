#ifndef PYRAMID_H
#define PYRAMID_H

#include "../VertexArrayObject.h"

class Pyramid : public VertexArrayObject {
public:
//const float size, const float height
	Pyramid(const float size, const float height);
	Pyramid();
	void draw();

	};

#endif // PYRAMID_H

