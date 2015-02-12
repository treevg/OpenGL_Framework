#ifndef MESHVAO_H
#define MESHVAO_H

#include "../ShaderTools/VertexArrayObject.h"

class Mesh : public VertexArrayObject
{
    private:
        unsigned int triangleCount;
        void createElementArrayBuffer(std::vector<GLint> values);
        void createArrayBuffer(unsigned int pointerIndex,
                               unsigned int valueCount,
                               std::vector<GLfloat> values);
    public:
        Mesh(std::vector<GLfloat> vertices = std::vector<GLfloat>(),
             std::vector<GLint>   indices  = std::vector<GLint>(),
             std::vector<GLfloat> normals  = std::vector<GLfloat>(),
             std::vector<GLfloat> uvs      = std::vector<GLfloat>(),
             std::vector<GLfloat> colors   = std::vector<GLfloat>());
        void draw();
};

#endif // MESHVAO_H
