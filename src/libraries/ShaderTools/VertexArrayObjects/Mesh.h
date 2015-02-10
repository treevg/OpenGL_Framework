#ifndef MESH_H
#define MESH_H


#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "../VertexArrayObject.h"

using namespace std;
using namespace glm;



struct Texture {
GLuint id;
string type;
};


class Mesh : public VertexArrayObject {
public:
	Mesh(vector<vec3> verticies, vector<vec3> normals, vector<vec2> textCoord,  vector<GLuint> indicies);
	Mesh(vector<vec3> verticies, vector<vec3> normals, vector<vec2> textCoord,  vector<GLuint> indicies, vector<Texture> textures);
	void draw();
	vector<vec3> getVerticies() const;
	vector<vec3> getNormals() const;
	vector<vec2> getTexCoords() const;
	vector<GLuint> getIndicies() const;
	vector<Texture> getTextures() const;
	
protected: 

vector<vec3> m_verticies;
vector<vec3> m_normals;
vector<vec2> m_texCoords;
vector<GLuint> m_indicies;
vector<Texture> m_textures;



	};

#endif // MESH_H
