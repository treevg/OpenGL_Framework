#ifndef MESHVAO_H
#define MESHVAO_H

#include "../ShaderTools/VertexArrayObject.h"

class Mesh : public VertexArrayObject
{
    private:
		std::vector<GLfloat> m_vertices;
		std::vector<GLint> m_indices;
        unsigned int triangleCount;
        unsigned int materialIndex = -1;
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
		inline std::vector<GLfloat>* getVertexList() { return &m_vertices; };
		inline std::vector<GLint>* getIndexList() { return &m_indices; };
        inline void setMaterialIndex(unsigned int index){ materialIndex = index; };
        inline int getMaterialIndex(){ return materialIndex; };
};

#endif // MESHVAO_H
