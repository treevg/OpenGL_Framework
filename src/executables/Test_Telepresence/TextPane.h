#define GLM_FORCE_RADIANS
#include <string>
#include "ShaderTools\VertexArrayObject.h"

class TextTexture;

class TextPane : public VertexArrayObject
{
public: 
	TextPane( float width, float height, std::string title);
	~TextPane();

	void draw();
	std::vector<glm::vec3> getVertices();

	GLuint getTextureHandle();

	glm::vec3 getCenter();
	glm::vec3 getNormal();

private:
	TextTexture* m_textTexture;
	std::vector<glm::vec3> m_vertices;
	glm::vec3 m_center;
	glm::vec3 m_normal;

	void createGeometry(float width, float height);
};