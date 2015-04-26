#ifndef VERTEX_ARRAY_OBJECT_H
#define VERTEX_ARRAY_OBJECT_H

#include <GL/glew.h>

#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <fstream>
#define GLM_FORCE_RADIANS
#include <glm/ext.hpp>
#include <map>

using namespace glm;
using namespace std;

struct MeshTexture {
GLuint id;
char type;
std::string path;
     };

struct Material {

vec3 ka;
vec3 kd;
vec3 ks;

     };



class VertexArrayObject {
public:
	VertexArrayObject* setMode(GLenum mode);
	virtual void draw() = 0;
  
    std::vector<MeshTexture> getTextures() const;
    Material getMaterial() const;
    void setModelMatrix(mat4 matrix);
    vector<vec3> getVertices() const;
     vector<vec3> getNormals() const;
     vector<vec2> getTexCoords() const;
     vector<GLuint> getIndices() const;
 

     mat4 modelMatrix;
     vec4 color;
     GLuint textureHandle;
     vector<vec3> m_vertices;
     vector<vec3> m_normals;
     vector<vec2> m_texCoords;
     vector<GLuint> m_indices;
     vector<MeshTexture> m_textures;
     Material material;
     string shaderProgramKey;
protected:
	GLuint vertexArrayObjectHandle;
	GLenum mode;
};

#endif // VERTEX_ARRAY_OBJECT_H
