#include "TextTexture.h"

TextTexture::TextTexture(int textureWidth, int textureHeight, const char *string, const GdkRGBA& textColor, const GdkRGBA& background, int fontSize)
{
	m_textColor = textColor;
	m_backgroundColor = background;
	m_string = string;
	m_width = textureWidth;
	m_height = textureHeight;
	m_fontSize = fontSize;

	drawText();
}

TextTexture::~TextTexture()
{

}


GLuint TextTexture::getTextureHandle()
{
	return m_textureId;
}


cairo_t* TextTexture::createCairoContext(int width, int height, int channels, cairo_surface_t** surf, unsigned char** buffer)
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


int TextTexture::drawText()
{
	cairo_surface_t* surface = NULL;
	cairo_t* cr;
	unsigned char* surfData;


	surface = cairo_image_surface_create_from_png("..\..\..\FP14\src\executables\Test_Telepresence\test.png");

	/* create cairo-surface/context to act as OpenGL-texture source */
	cr = createCairoContext(m_width, m_height, 4, &surface, &surfData);


	/* clear background */
	//cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
	//cairo_set_source_rgb(cr, m_backgroundColor.red, m_backgroundColor.green, m_backgroundColor.blue);
	cairo_paint(cr);

	//TODO use proper positioning
	cairo_move_to(cr, m_width / 10, m_height / 2);
	cairo_set_font_size(cr, m_fontSize);
	cairo_select_font_face(cr, "Lucida Console", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_source_rgb(cr, m_textColor.red, m_textColor.green, m_textColor.blue);
	cairo_show_text(cr, m_string.c_str());

	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, surfData);

	free(surfData);
	cairo_destroy(cr);

	return m_textureId;
}