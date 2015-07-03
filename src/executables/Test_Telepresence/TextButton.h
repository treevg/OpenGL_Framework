#include <stdio.h>
#include <string>
#include "ShaderTools\VertexArrayObjects\Cube.h"
#include "cairo\cairo.h"
#include "gtk\gtk.h"

class TextTexture;

class TextButton : public Cube
{
public: 
	TextButton( glm::vec3 position, float size, std::string title );
	~TextButton();

	GLuint getTextureHandle();

private:
	TextTexture* m_textTexture;
	//GLuint m_textureId;

	//cairo_t* createCairoContext(int width, int height, int channels, cairo_surface_t** surf, unsigned char** buffer);
	//int drawText(int x, int y, int width, int height, const char *string, GdkRGBA& textColor, GdkRGBA& background);
};