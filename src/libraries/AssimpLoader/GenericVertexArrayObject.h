#ifndef GENERICVAO_H
#define GENERICVAO_H

#include "../ShaderTools/VertexArrayObject.h"

class GenericVertexArrayObject : public VertexArrayObject
{
    private:
        unsigned int triangleCount;
    public:
        GenericVertexArrayObject();
        void draw();
};

#endif // GENERICVAO_H
