#include "Mesh.h"

Mesh::Mesh(std::vector<GLfloat> vertices,
           std::vector<GLint>   indices,
           std::vector<GLfloat> normals,
           std::vector<GLfloat> uvs,
           std::vector<GLfloat> colors)
{
    const float size = 1.0f;
    mode = GL_TRIANGLES;
    triangleCount = indices.size();

    glGenVertexArrays(1, &vertexArrayObjectHandle);
    glBindVertexArray(vertexArrayObjectHandle);

    if(vertices.size() != 0)
    {
        createArrayBuffer(0, 3, vertices);
    }
    if(indices.size() != 0)
    {
        createElementArrayBuffer(indices);
    }
    if(normals.size() != 0)
    {
        createArrayBuffer(1, 3, normals);
    }
    if(uvs.size() != 0)
    {
        createArrayBuffer(2, 2, uvs);
    }
    if(colors.size() != 0)
    {
        createArrayBuffer(3, 3, colors);
    }
}


void Mesh::createElementArrayBuffer(std::vector<GLint> values)
{
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, values.size() * sizeof(GLint), &values[0], GL_STATIC_DRAW);
}

void Mesh::createArrayBuffer(unsigned int pointerIndex,
                               unsigned int valueCount,
                               std::vector<GLfloat> values)
{
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, values.size() * sizeof(GLfloat), &values[0], GL_STATIC_DRAW);
    glVertexAttribPointer(pointerIndex, valueCount, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);
}

void Mesh::draw()
{
    glBindVertexArray(vertexArrayObjectHandle);
    glDrawElements(GL_TRIANGLES, triangleCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
