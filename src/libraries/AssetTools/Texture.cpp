#include "Texture.h"
#include <vector>

Texture::Texture(std::string path) {

	textureHandle = load(path);
}

Texture::Texture(int w, int h) {
    this->w = w;
    this->h = h;
    textureHandle = this->genTexture(w, h);
}

Texture::~Texture(){

}

GLuint Texture::getHandle() {
	return textureHandle;
}

GLuint Texture::load(std::string path) {
	if (!devILInitialized) {
		ilInit();
		iluInit();
		ilutRenderer(ILUT_OPENGL);
		devILInitialized = true;
	}

    ILuint iid;
    ilGenImages(1, &iid);
    ilBindImage(iid);
    ilLoadImage(path.c_str());

    int w = ilGetInteger(IL_IMAGE_WIDTH);
    int h = ilGetInteger(IL_IMAGE_HEIGHT);
    const int byteCount = sizeof(float) * 3 * w * h;
    unsigned char* pixels = new unsigned char[byteCount];
    ilCopyPixels(0, 0, 0, w, h, 1, IL_RGB, IL_FLOAT, pixels);

    GLuint t;
    glGenTextures(1, &t);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGB, GL_FLOAT, pixels);
    glBindImageTexture(0, t, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA16F);

    return t;
}

void Texture::clear() {
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureHandle);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, pixels);
//    glBindImageTexture(0, textureHandle, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA16F);
}

GLuint Texture::genTexture(int w, int h) {

    byteCount = sizeof(float) * 4 * w * h;
    pixels = new unsigned char[byteCount];

    float f = 1000.0f;

    unsigned char const * p = reinterpret_cast<unsigned char const *>(&f);

    for (int i = 0; i < byteCount; i+=4)
    {
        pixels[i] = p[0];
        pixels[i+1] = p[1];
        pixels[i+2] = p[2];
        pixels[i+3] = p[3];
    }

    GLuint t;
    glGenTextures(1, &t);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, pixels);
    glBindImageTexture(0, t, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA16F);

    return t;
}
