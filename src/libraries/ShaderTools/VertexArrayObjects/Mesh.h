#ifndef MESH_H
#define MESH_H

#include <assert.h>
#include <vector>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include "../VertexArrayObject.h"

using namespace std;
using namespace glm;



struct MeshTexture {
GLuint id;
string type;
string path;
};


class Mesh: public VertexArrayObject  {

public:
	Mesh(vector<vec3> vertices, vector<vec3> normals, vector<vec2> textCoord,  vector<GLuint> indices);
	Mesh(vector<vec3> vertices, vector<vec3> normals, vector<vec2> textCoord,  vector<GLuint> indices, vector<MeshTexture> textures);
	void draw();
	void printInd();
	vector<vec3> getVertices() const;
	vector<vec3> getNormals() const;
	vector<vec2> getTexCoords() const;
	vector<GLuint> getIndices() const;
	vector<MeshTexture> getTextures() const;

	
	
protected: 

vector<vec3> m_vertices;
vector<vec3> m_normals;
vector<vec2> m_texCoords;
vector<GLuint> m_indices;
vector<MeshTexture> m_textures;

GLuint IBO;
GLuint VBO;
GLuint tVBO;


	};

#endif // MESH_H
