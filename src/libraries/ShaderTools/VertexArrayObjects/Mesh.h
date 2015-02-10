#ifndef MESH_H
#define MESH_H


#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "../VertexArrayObject.h"

using namespace std;
using namespace glm;


struct Vertex {

vec3 position;
vec3 normal;
vec2 textCoordin;

};

struct Texture {
GLuint id;
string type;
};


class Mesh : public VertexArrayObject {
public:
	Mesh(vector<Vertex> verticies, vector<GLuint> indicies, vector<Texture> textures);
	void draw();
	vector<Vertex> getVerticies() const;
	vector<GLuint> getIndicies() const;
	vector<Texture> getTextures() const;
	
protected: 

vector<Vertex> verticies;
vector<GLuint> indicies;
vector<Texture> textures;


	};

#endif // MESH_H
