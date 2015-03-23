#include <vector>
#include "glm\glm.hpp"
#include "glm\gtc\constants.hpp"
#include "GL\glew.h"
#include <GLFW/glfw3.h>
#include "../VertexArrayObject.h"

#define INVALID_OGL_VALUE 0xFFFFFFFF

class Sphere : public VertexArrayObject {

public:
	Sphere();
	Sphere(float radius);
	Sphere(glm::vec3 center, float radius);
	Sphere(float radius, int resolution);
	Sphere(glm::vec3 center, float radius, int resolution);
	~Sphere();

	glm::vec3 get_center();
	float get_radius();

	void draw();

private:
	void create(glm::vec3 center, float radius, int resolution);

	glm::vec3 m_Center;
	float m_radius;
	int m_resolution;

	int m_geotype;
	GLuint m_vertexbuffer;
	GLuint m_normalbuffer;
	GLuint m_indexlist;

	int m_points;
	int m_indices;

	std::vector<glm::vec3> m_vertices; // vertice positions
	std::vector<glm::vec3> m_normals; // normals
	std::vector<unsigned int> m_index; //index map
};