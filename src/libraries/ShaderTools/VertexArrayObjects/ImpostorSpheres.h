#ifndef ImpostorSpheres_H
#define ImpostorSpheres_H

#include "../VertexArrayObject.h"

class ImpostorSpheres : public VertexArrayObject {
public:
    ImpostorSpheres();
    void draw();
    void drawInstanced(int countInstances);
    int num_balls;
};

#endif // ImpostorSpheres_H
