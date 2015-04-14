#include "Sphere.h"

Sphere::Sphere()
{
	m_radius = 1.0f;
	m_resolution = 20;
	m_Center = glm::vec3(0);
	create(m_Center, m_radius, m_resolution);
}

Sphere::Sphere(float radius)
{
	m_radius = radius;
	m_resolution = 20;
	m_Center = glm::vec3(0);
	create(m_Center, m_radius, m_resolution);
}

Sphere::Sphere(glm::vec3 center, float radius)
{
	m_radius = radius;
	m_resolution = 20;
	m_Center = center;
	create(m_Center, m_radius, m_resolution);
}

Sphere::Sphere(float radius, int resolution)
{
	m_radius = radius;
	m_resolution = resolution;
	m_Center = glm::vec3(0);
	create(m_Center, m_radius, m_resolution);
}

Sphere::Sphere(glm::vec3 center, float radius, int resolution)
{
	m_radius = radius;
	m_resolution = resolution;
	m_Center = center;
	create(m_Center, m_radius, m_resolution);
}

Sphere::~Sphere()
{
}

glm::vec3 Sphere::get_center()
{
	return m_Center;
}

float Sphere::get_radius()
{
	return m_radius;
}

void Sphere::create(glm::vec3 center, float radius, int resolution)
{
	// iniatialize the variable we are going to use
	float u, v;
	float phi, theta;
	float x, y, z;
	int offset = 0, i, j;

	// create points
	for (j = 0; j <= resolution; j++)  //theta
		for (i = 0; i <= resolution; i++) //phi
		{
			u = i / (float)resolution;		phi = 2 * glm::pi<float>() * u;
			v = j / (float)resolution;		theta = glm::pi<float>() * v;

			x = m_Center.x + radius * sin(theta) * sin(phi);
			y = m_Center.y + radius * cos(theta);
			z = m_Center.z + radius * sin(theta) * cos(phi);

			m_vertices.push_back(glm::vec3(x, y, z));
			m_normals.push_back(glm::vec3(x, y, z) / radius);
		}
	m_points = m_vertices.size();

	// create index list
	for (j = 0; j < resolution; j++)
	{
		for (i = 0; i < resolution; i++)
		{
			// 1. Triangle
			m_index.push_back(offset + i);
			m_index.push_back(offset + i + resolution + 1);
			m_index.push_back(offset + i + resolution + 1 + 1);

			// 2. Triangle
			m_index.push_back(offset + i + resolution + 1 + 1);
			m_index.push_back(offset + i + 1);
			m_index.push_back(offset + i);
		}
		offset += resolution + 1;
	}
	m_indices = m_index.size();

	m_points = m_vertices.size();
	m_indices = m_index.size();




	glGenVertexArrays(1, &vertexArrayObjectHandle);
	glBindVertexArray(vertexArrayObjectHandle);

	GLuint vertexBufferHandles[2];
	glGenBuffers(2, vertexBufferHandles);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[0]);
	glBufferData(GL_ARRAY_BUFFER, m_points * sizeof(glm::vec3), &m_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[1]);
	glBufferData(GL_ARRAY_BUFFER, m_points * sizeof(glm::vec3), &m_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// Generate a buffer for the indices as well 
	glGenBuffers(1, &m_indexlist);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexlist);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices * sizeof(unsigned int), &m_index[0], GL_STATIC_DRAW);
}

void Sphere::draw() {
	glBindVertexArray(vertexArrayObjectHandle);
	glDrawElements(GL_TRIANGLES, m_indices, GL_UNSIGNED_INT, 0);
}
