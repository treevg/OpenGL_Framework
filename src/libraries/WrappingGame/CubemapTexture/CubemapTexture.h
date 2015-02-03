#ifndef CUBEMAPTEXTURE_H
#define CUBEMAPTEXTURE_H

#include  "stb_image.h"
#include   <string>


using namespace std; 

static const GLenum cubeFaces[6]=  { GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	                               GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	                               GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	                               GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	                               GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	                               GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

class CubemapTexture {

public:
	CubemapTexture ();
	~CubemapTexture();
	bool loadTexture(GLuint textureId, GLenum face_target, string file);
	GLuint create_cube_map(const string& r, const string& l,const string& up, 
	                                const string& down,const string& back, const string& front);


//protected:
	GLuint m_textureHandler;
	string  m_images[6];

};

#endif // CUBEMAPTEXTURE_H