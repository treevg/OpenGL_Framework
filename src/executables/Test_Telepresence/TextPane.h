#define GLM_FORCE_RADIANS
#include <string>
#include "ShaderTools\VertexArrayObject.h"

class TextTexture;

class TextPane : public VertexArrayObject
{
public: 
	TextPane(glm::vec3 position, float width, float height, std::string title);
	~TextPane();

	void draw();
	std::vector<glm::vec3> getVertices();

	GLuint getTextureHandle();

	glm::vec3 getCenter();
	glm::mat4 getBillboardModelMatrix();
	glm::mat4 getBillboardModelMatrix(glm::vec3 cameraPosition);

private:
	TextTexture* m_textTexture;
	std::vector<glm::vec3> m_vertices;
	glm::vec3 m_center;

	glm::quat rotationBetweenVectors(glm::vec3 start, glm::vec3 dest);

	void createGeometry(float width, float height);
};