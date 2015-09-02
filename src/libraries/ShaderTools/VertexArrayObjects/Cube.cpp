#include "Cube.h"

Cube::Cube() {
	const float size = 0.5;

    glGenVertexArrays(1, &vertexArrayObjectHandle);
    glBindVertexArray(vertexArrayObjectHandle);

    
	GLuint vertexBufferHandles[2];
    glGenBuffers(2, vertexBufferHandles);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[0]);

    float positions[] = {
		// Front face
		-size, -size, size, size, -size, size, size, size, size,
		size, size, size, -size, size, size, -size, -size, size,
		// Right face
		size, -size, size, size, -size, -size, size, size, -size,
		size, size, -size, size, size, size, size, -size, size,
		// Back face
		-size, -size, -size, size, -size, -size, size, size, -size,
		size, size, -size, -size, size, -size, -size, -size, -size,
		// Left face
		-size, -size, size, -size, -size, -size, -size, size, -size,
		-size, size, -size, -size, size, size, -size, -size, size,
		// Bottom face
		-size, -size, size, size, -size, size, size, -size, -size,
		size, -size, -size, -size, -size, -size, -size, -size, size,
		// Top Face
		-size, size, size, size, size, size, size, size, -size,
		size, size, -size, -size, size, -size, -size, size, size,
    };

	for (int v = 0; v < sizeof(positions); v+=3)
		vertices.push_back(glm::vec3(positions[v], positions[v + 1], positions[v + 2]));

    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    GLfloat normals[] = {
		// Front face
		0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
		// Right face
		1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
		1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
		// Back face
		0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0,
		0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0,
		// Left face
		-1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0,
		-1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0,
		// Bottom face
		0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0,
		0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0,
		// Top face
		0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
		0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0
    };

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

	//GLfloat tangents[] = {
	//	// Front face
	//	0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0,
	//	0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0,
	//	// Right face
	//	0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0,
	//	0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0,
	//	// Back face
	//	1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
	//	1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
	//	// Left face
	//	0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
	//	0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
	//	// Bottom face
	//	0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
	//	0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0
	//	// Top face
	//	- 1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0,
	//	-1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0

	//};

	//GLfloat uvCoordinates[] = {
	//	// Front face
	//	0, 0, 1, 0, 1, 1,
	//	1, 1, 0, 1, 0, 0,
	//	// Right face
	//	0, 0, 1, 0, 1, 1,
	//	1, 1, 0, 1, 0, 0,
	//	// Back face
	//	0, 0, 1, 0, 1, 1,
	//	1, 1, 0, 1, 0, 0,
	//	// Left face
	//	0, 0, 1, 0, 1, 1,
	//	1, 1, 0, 1, 0, 0,
	//	// Bottom face
	//	0, 0, 1, 0, 1, 1,
	//	1, 1, 0, 1, 0, 0,
	//	// Top face
	//	0, 0, 1, 0, 1, 1,
	//	1, 1, 0, 1, 0, 0,
	//};
}

Cube::Cube(glm::vec3 position, float size) {
	printf("Constructing CUBULUS!");

	glGenVertexArrays(1, &vertexArrayObjectHandle);
	glBindVertexArray(vertexArrayObjectHandle);

	GLuint vertexBufferHandles[3];
	glGenBuffers(3, vertexBufferHandles);

	glm::vec3 p0 = glm::vec3(position.x - size, position.y + size, position.z + size);
	glm::vec3 p1 = glm::vec3(position.x - size, position.y - size, position.z + size);
	glm::vec3 p2 = glm::vec3(position.x + size, position.y + size, position.z + size);
	glm::vec3 p3 = glm::vec3(position.x + size, position.y - size, position.z + size);

	glm::vec3 p4 = glm::vec3(position.x - size, position.y + size, position.z - size);
	glm::vec3 p5 = glm::vec3(position.x - size, position.y - size, position.z - size);
	glm::vec3 p6 = glm::vec3(position.x + size, position.y + size, position.z - size);
	glm::vec3 p7 = glm::vec3(position.x + size, position.y - size, position.z - size);

	float positions[] = {
		//Front
		p0.x, p0.y, p0.z, p1.x, p1.y, p1.z, p3.x, p3.y, p3.z,
		p0.x, p0.y, p0.z, p3.x, p3.y, p3.z, p2.x, p2.y, p2.z,

		//Right
		p2.x, p2.y, p2.z, p3.x, p3.y, p3.z, p7.x, p7.y, p7.z,
		p2.x, p2.y, p2.z, p7.x, p7.y, p7.z, p6.x, p6.y, p6.z,

		//Back
		p6.x, p6.y, p6.z, p7.x, p7.y, p7.z, p5.x, p5.y, p5.z,
		p6.x, p6.y, p6.z, p5.x, p5.y, p5.z, p4.x, p4.y, p4.z,

		//Left
		p4.x, p4.y, p4.z, p5.x, p5.y, p5.z, p1.x, p1.y, p1.z,
		p4.x, p4.y, p4.z, p1.x, p1.y, p1.z, p0.x, p0.y, p0.z,

		//Top
		p0.x, p0.y, p0.z, p6.x, p6.y, p6.z, p4.x, p4.y, p4.z,
		p0.x, p0.y, p0.z, p2.x, p2.y, p2.z, p6.x, p6.y, p6.z,

		//Bottom
		p5.x, p5.y, p5.z, p3.x, p3.y, p3.z, p1.x, p1.y, p1.z,
		p5.x, p5.y, p5.z, p7.x, p7.y, p7.z, p3.x, p3.y, p3.z,
	};
	//float positions[] = {
	//	// Front face
	//	position.x - size, position.y - size, position.z + size, position.x + size, position.y - size, position.z + size, position.x + size, position.y + size, position.z + size,
	//	position.x + size, position.y + size, position.z + size, position.x - size, position.y + size, position.z + size, position.x - size, position.y - size, position.z + size,
	//	// Right face
	//	position.x + size, position.y - size, position.z + size, position.x + size, position.y - size, position.z - size, position.x + size, position.y + size, position.z - size,
	//	position.x + size, position.y + size, position.z - size, position.x + size, position.y + size, position.z + size, position.x + size, position.y - size, position.z + size,
	//	// Back face
	//	position.x - size, position.y - size, position.z - size, position.x + size, position.y - size, position.z - size, position.x + size, position.y + size, position.z - size,
	//	position.x + size, position.y + size, position.z - size, position.x - size, position.y + size, position.z - size, position.x - size, position.y - size, position.z - size,
	//	// Left face
	//	position.x - size, position.y - size, position.z + size, position.x - size, position.y - size, position.z - size, position.x - size, position.y + size, position.z - size,
	//	position.x - size, position.y + size, position.z - size, position.x - size, position.y + size, position.z + size, position.x - size, position.y - size, position.z + size,
	//	// Bottom face
	//	position.x - size, position.y - size, position.z + size, position.x + size, position.y - size, position.z + size, position.x + size, position.y - size, position.z - size,
	//	position.x + size, position.y - size, position.z - size, position.x - size, position.y - size, position.z - size, position.x - size, position.y - size, position.z + size,
	//	// Top Face
	//	position.x - size, position.y + size, position.z + size, position.x + size, position.y + size, position.z + size, position.x + size, position.y + size, position.z - size,
	//	position.x + size, position.y + size, position.z - size, position.x - size, position.y + size, position.z - size, position.x - size, position.y + size, position.z + size,
	//};

	for (int v = 0; v < sizeof(positions); v += 3)
		vertices.push_back(glm::vec3(positions[v], positions[v + 1], positions[v + 2]));

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	m_vertexCount = vertices.size();

	GLfloat normals[] = {
		// Front face
		0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
		// Right face
		1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
		1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
		// Back face
		0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0,
		0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0,
		// Left face
		-1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0,
		-1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0,
		// Bottom face
		0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0,
		0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0,
		// Top face
		0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
		0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0
	};
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);



	GLfloat uvCoordinates[] = {
		// Front face
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		// Right face
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		// Back face
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		// Left face
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		// Bottom face
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		// Top face
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f
	};
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvCoordinates), uvCoordinates, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	printf("Finished Constructing CUBULUS!");
}

void Cube::draw() {
    glBindVertexArray(vertexArrayObjectHandle);
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
}

std::vector<glm::vec3> Cube::getVertices(){
	return vertices;
}