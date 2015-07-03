#pragma once

#include <GL\glew.h>
#include "ShaderTools\VertexArrayObject.h"
#include "cairo\cairo.h"
#include "gtk\gtk.h"

class TextTexture
{
public:
	virtual GLuint getTextureHandle();
protected:
	GLuint m_textureId;
	cairo_t* createCairoContext(int width, int height, int channels, cairo_surface_t** surf, unsigned char** buffer);
	int drawText(int x, int y, int width, int height, const char *string, GdkRGBA& textColor, GdkRGBA& background);
};

