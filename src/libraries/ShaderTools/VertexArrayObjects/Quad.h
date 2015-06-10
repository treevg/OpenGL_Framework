#ifndef QUAD_H
#define QUAD_H

#include "../VertexArrayObject.h"

class Quad : public VertexArrayObject {
public:
	Quad();
    void draw();
    void drawInstanced(int countInstances);
    void prepareInstancedDrawing();
};

#endif // QUAD_H
