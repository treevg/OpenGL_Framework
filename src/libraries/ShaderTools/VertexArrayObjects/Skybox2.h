#ifndef SKYBOX2_H
#define SKYBOX2_H


//THIS SKYBOX IS T USE WITH WARPPING/SKY.VERT & WARPPING/SKY.FRAG

//ATTRIBUTE LESS RENDERING
#include "../VertexArrayObject.h"

class Skybox2: public VertexArrayObject {
public:
	Skybox2();
	void draw();
};

#endif // SKYBOX2_H
