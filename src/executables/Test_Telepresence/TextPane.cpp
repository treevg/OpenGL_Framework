#include "TextPane.h"
#include "TextTexture.h"


TextPane::TextPane(glm::vec3 position, float width, float height, std::string title)
{
	printf("Constructing TextButton!");
	GdkRGBA color = { 1.0f, 1.0f, 1.0f, 1.0f };
	GdkRGBA background = { 0.5f, 0.5f, 0.0f, 0.0f };
	
	m_textTexture = new TextTexture(256, 256, title.c_str(), color, background);
	createGeometry(position, width, height);
}

TextPane::~TextPane()
{
	delete m_textTexture;
}


GLuint TextPane::getTextureHandle()
{
	return m_textTexture->getTextureHandle();
}

void TextPane::createGeometry(glm::vec3 position, float width, float height)
{
	printf("Constructing PANE!");
	mode = GL_TRIANGLES;

	glGenVertexArrays(1, &vertexArrayObjectHandle);
	glBindVertexArray(vertexArrayObjectHandle);

	GLuint vertexBufferHandles[3];
	glGenBuffers(3, vertexBufferHandles);

	glm::vec3 p0( position );
	glm::vec3 p1(position.x, position.y-height, position.z);
	glm::vec3 p2(position.x + width, position.y, position.z);
	glm::vec3 p3(position.x + width, position.y - height, position.z);

	float positions[] = {
		// Front face
		p0.x, p0.y, p0.z, p1.x, p1.y, p1.z, p2.x, p2.y, p2.z,
		p1.x, p1.y, p1.z, p3.x, p3.y, p3.z, p2.x, p2.y, p2.z,
	};

	for (int v = 0; v < sizeof(positions); v += 3)
	{
		m_vertices.push_back(glm::vec3(positions[v], positions[v + 1], positions[v + 2]));
	}

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	GLfloat normals[] = {
		// Front face
		0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
	};
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);



	GLfloat uvCoordinates[] = {
		// Front face
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	};
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvCoordinates), uvCoordinates, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	printf("Finished Constructing CUBULUS!");
}


void TextPane::draw() {
	glBindVertexArray(vertexArrayObjectHandle);
	glDrawArrays(mode, 0, 2 * 3);
}


std::vector<glm::vec3> TextPane::getVertices(){
	return m_vertices;
}

