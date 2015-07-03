#include <stdio.h>
#include <string>
#include "ShaderTools\VertexArrayObject.h"
#include "cairo\cairo.h"
#include "gtk\gtk.h"

class TextTexture;

class TextPane : public VertexArrayObject
{
public: 
	TextPane(glm::vec3 position, float width, float height, std::string title);
	~TextPane();

	void draw();
	std::vector<glm::vec3> getVertices();

	GLuint getTextureHandle();

private:
	TextTexture* m_textTexture;
	std::vector<glm::vec3> m_vertices;

	void createGeometry(glm::vec3 position, float width, float height);
};