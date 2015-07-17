#include "TextButton.h"
#include "TextTexture.h"


TextButton::TextButton( glm::vec3 position, float size, std::string title)
	:Cube(position, size)
{
	printf("Constructing TextButton!");
	GdkRGBA color = { 1.0f, 1.0f, 1.0f, 1.0f };
	GdkRGBA background = { 0.0f, 0.0f, 0.0f, 0.0f };
	
	m_textTexture = new TextTexture(256, 256, title.c_str(), color, background);
}

TextButton::~TextButton()
{
	delete m_textTexture;
}


GLuint TextButton::getTextureHandle()
{
	return m_textTexture->getTextureHandle();
}

