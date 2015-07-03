#include "TextButton.h"


TextButton::TextButton( glm::vec3 position, float size, std::string title)
	:Cube(position, size)
{
	printf("Constructing TextButton!");
	GdkRGBA color = { 1.0f, 1.0f, 1.0f, 1.0f };
	GdkRGBA background = { 0.0f, 0.0f, 0.0f, 0.0f };

	drawText(1, 0, 0, 1, title.c_str(), color, background);
	printf("Finished Constructing TextButton!");
}


GLuint TextButton::getTextureHandle()
{
	return m_textureId;
}


cairo_t* TextButton::createCairoContext(int width, int height, int channels, cairo_surface_t** surf, unsigned char** buffer)
{
	cairo_t* cr;

	/* create cairo-surface/context to act as OpenGL-texture source */
	*buffer = (unsigned char*)calloc(channels * width * height, sizeof(unsigned char));
	if (!*buffer)
	{
		printf("create_cairo_context() - Couldn't allocate surface-buffer\n");
		return NULL;
	}

	*surf = cairo_image_surface_create_for_data(*buffer, CAIRO_FORMAT_ARGB32, width, height, channels * width);
	if (cairo_surface_status(*surf) != CAIRO_STATUS_SUCCESS)
	{
		free(*buffer);
		printf("create_cairo_context() - Couldn't create surface\n");
		return NULL;
	}

	cr = cairo_create(*surf);
	if (cairo_status(cr) != CAIRO_STATUS_SUCCESS)
	{
		free(*buffer);
		printf("create_cairo_context() - Couldn't create context\n");
		return NULL;
	}

	return cr;
}


int TextButton::drawText(int x, int y, int width, int height, const char *string, GdkRGBA& textColor, GdkRGBA& background)
{
	cairo_surface_t* surface = NULL;
	cairo_t* cr;
	unsigned char* surfData;

	/* create cairo-surface/context to act as OpenGL-texture source */
	cr = createCairoContext(256, 256, 4, &surface, &surfData);

	/* clear background */
	cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
	cairo_set_source_rgb(cr, background.red, background.green, background.blue);
	cairo_paint(cr);

	cairo_move_to(cr, 256 / 10, 256 / 2);
	cairo_set_font_size(cr, 30);
	cairo_select_font_face(cr, "sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_source_rgb(cr, textColor.red, textColor.green, textColor.blue);
	cairo_show_text(cr, string);

	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, surfData);

	//TestEGLError("glTexImage2D");

	free(surfData);
	cairo_destroy(cr);

	return m_textureId;
}