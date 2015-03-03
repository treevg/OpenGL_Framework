
#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assert.h>

#include "CubemapTexture.h"

using namespace std;

CubemapTexture::CubemapTexture(){

}

CubemapTexture::~CubemapTexture(){

}

	bool CubemapTexture::loadTexture(GLuint textureId, GLenum face_target, string file){
		bool loaded = false;
		//create Texture 
        
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);


        int width, height, bytesPerPixel;
        unsigned char *data = stbi_load(file.c_str(), &width, &height, &bytesPerPixel, 0);
        cout << "bytes per px " << bytesPerPixel << endl;
        //send image data to the new texture
        if (!data) {
            std::cout << "ERROR: Unable to open image"  << file << std::endl;
            loaded = false;
        } else if (bytesPerPixel == 3){
            glTexImage2D(face_target, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
               loaded = true;

        } else if (bytesPerPixel == 4) {
            glTexImage2D(face_target, 0,GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            loaded = true; 
        } else {
            std::cout << "Unknown format for bytes per pixel... Changed to \"4\"" << std::endl;
            glTexImage2D(face_target, 0,GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            loaded = true;
        }

        stbi_image_free(data);
  
     if (loaded)   std::cout << "SUCCESS: image loaded from " << file << std::endl;


return loaded;

	

}

GLuint CubemapTexture::create_cube_map(const string& r, const string& l,const string& up, 
	                                const string& down,const string& back, const string& front){

	GLuint  tex_cube;
    glDepthMask(0);
    glActiveTexture (GL_TEXTURE0);
    glGenTextures (1, &tex_cube);

assert (loadTexture (tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_X, r));
assert (loadTexture (tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, l));
assert (loadTexture (tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, up));
assert (loadTexture (tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, down));
assert (loadTexture (tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back));
assert (loadTexture (tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front));


glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glDepthMask(1);


return  tex_cube;

}

