#include "CustomZBuffer.h"
#include <vector>



CustomZBuffer::CustomZBuffer(int w, int h) {
    this->w = w;
    this->h = h;
    textureHandle = this->genTexture(w, h);
}

CustomZBuffer::~CustomZBuffer(){

}

GLuint CustomZBuffer::getHandle() {
	return textureHandle;
}

void CustomZBuffer::clear() {
    glBindTexture(GL_TEXTURE_2D, textureHandle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, pixels);
}

GLuint CustomZBuffer::genTexture(int w, int h) {

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
