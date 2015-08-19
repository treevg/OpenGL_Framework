#include "TextPane.h"
#include "TextTexture.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>


TextPane::TextPane(glm::vec3 position, float width, float height, std::string title)
{
	printf("Constructing TextButton!");
	GdkRGBA color = { 1.0f, 1.0f, 1.0f, 1.0f };
	GdkRGBA background = { 0.5f, 0.5f, 0.0f, 0.0f };

	m_center = glm::vec3(position);
	
	m_textTexture = new TextTexture(256, 256, title.c_str(), color, background);
	createGeometry(width, height);
}

TextPane::~TextPane()
{
	delete m_textTexture;
}


GLuint TextPane::getTextureHandle()
{
	return m_textTexture->getTextureHandle();
}

glm::vec3 TextPane::getCenter()
{
	return m_center;
}

glm::mat4 TextPane::getBillboardModelMatrix()
{
	glm::mat4 modelMatrix(1.0f);
	modelMatrix[0][3] = m_center[0];
	modelMatrix[1][3] = m_center[1];
	modelMatrix[2][3] = m_center[2];
	return modelMatrix;
}

glm::mat4 TextPane::getBillboardModelMatrix(glm::vec3 cameraPosition)
{
	glm::vec3 start(0.0f, 0.0f, -1.0f);
	glm::vec3 dest( cameraPosition + m_center );
	if (dest == glm::vec3(0.0f))
	{
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix[0][3] = m_center[0];
		modelMatrix[1][3] = m_center[1];
		modelMatrix[2][3] = m_center[2];
		return modelMatrix;
	}
	glm::quat rotation = rotationBetweenVectors( start, dest );
	glm::mat4 modelMatrix = glm::toMat4(rotation);
	modelMatrix[3][0] = m_center[0];
	modelMatrix[3][1] = m_center[1];
	modelMatrix[3][2] = m_center[2];
	return modelMatrix;
}

glm::quat TextPane::rotationBetweenVectors(glm::vec3 start, glm::vec3 dest)
{
	start = glm::normalize(start);
	dest = glm::normalize(dest);

	float cosTheta = glm::dot(start, dest);
	glm::vec3 rotationAxis;

	if (cosTheta < -1 + 0.001f){
		// special case when vectors in opposite directions:
		// there is no "ideal" rotation axis
		// So guess one; any will do as long as it's perpendicular to start
		rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
		if (glm::length2(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
			rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start);

		rotationAxis = glm::normalize(rotationAxis);
		return glm::angleAxis(180.0f, rotationAxis);
	}

	rotationAxis = glm::cross(start, dest);

	float s = sqrt((1 + cosTheta) * 2);
	float invs = 1 / s;

	return glm::quat(
		s * 0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
		);

}

void TextPane::createGeometry( float width, float height)
{
	printf("Constructing PANE!");
	mode = GL_TRIANGLES;

	glGenVertexArrays(1, &vertexArrayObjectHandle);
	glBindVertexArray(vertexArrayObjectHandle);

	GLuint vertexBufferHandles[3];
	glGenBuffers(3, vertexBufferHandles);

	float hw = width * 0.5f;
	float hh = height * 0.5f;
	glm::vec3 p0(-hw,  hh, 0.0);
	glm::vec3 p1(-hw, -hh, 0.0);
	glm::vec3 p2( hw,  hh, 0.0);
	glm::vec3 p3( hw, -hh, 0.0);

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

