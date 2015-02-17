#ifndef MESH_H
#define MESH_H


#include <vector>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include "../VertexArrayObject.h"

using namespace std;
using namespace glm;



struct Texture {
GLuint id;
string type;
};


class Mesh: public VertexArrayObject  {
public:
	Mesh(vector<vec3> vertices, vector<vec3> normals, vector<vec2> textCoord,  vector<GLuint> indices);
	Mesh(vector<vec3> vertices, vector<vec3> normals, vector<vec2> textCoord,  vector<GLuint> indices, vector<Texture> textures);
	void draw();
	void printInd();
	vector<vec3> getVertices() const;
	vector<vec3> getNormals() const;
	vector<vec2> getTexCoords() const;
	vector<GLuint> getIndices() const;
	vector<Texture> getTextures() const;

	
	
protected: 

vector<vec3> m_vertices;
vector<vec3> m_normals;
vector<vec2> m_texCoords;
vector<GLuint> m_indices;
vector<Texture> m_textures;

GLuint IBO;
GLuint VBO;
GLuint tVBO;


	};

#endif // MESH_H
