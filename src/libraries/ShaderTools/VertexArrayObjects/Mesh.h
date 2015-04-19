#ifndef MESH_H
#define MESH_H


#include <GL/glew.h>
#include <assert.h>
#include <vector>
#include <string>
#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "../VertexArrayObject.h"


using namespace std;
using namespace glm;

/*diffuse: d, ambient: a, specular: s*/




class Mesh  : public VertexArrayObject {

public:
	Mesh(vector<vec3> vertices, vector<vec3> normals, vector<vec2> textCoord,  vector<GLuint> indices, mat4 modelMatrix, string shaderProgramKey);
	Mesh(vector<vec3> vertices, vector<vec3> normals, vector<vec2> textCoord,  vector<GLuint> indices, vector<MeshTexture> textures, mat4 modelMatrix, Material mat, string shaderProgramKey);
	void draw();
	void printInd();
	vector<vec3> getVertices() const;
	vector<vec3> getNormals() const;
	vector<vec2> getTexCoords() const;
	vector<GLuint> getIndices() const;
	vector<MeshTexture> getTextures() const;

    GLuint getVertexArrayObject() const;
	
	
protected: 

/*vector<vec3> m_vertices;
vector<vec3> m_normals;
vector<vec2> m_texCoords;
vector<GLuint> m_indices; 
vector<MeshTexture> m_textures; */

GLuint IBO;
GLuint VBO;
GLuint tVBO;
GLuint ABO;


	};

#endif // MESH_H
