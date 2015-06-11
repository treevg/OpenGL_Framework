#ifndef CustomZBuffer_H
#define CustomZBuffer_H

#include <string>
#include <string.h>
#include <GL/glew.h>
#include <IL/ilu.h>
#include <IL/ilut.h>


class CustomZBuffer
{
public:
    CustomZBuffer(std::string path);
    CustomZBuffer(int w, int h);
    ~CustomZBuffer();
	GLuint getHandle();
    void clear();
    GLuint genTexture(int w, int h);

	static GLuint load(std::string path);

protected:
	GLuint textureHandle;
    int w;
    int h;
    int byteCount;
    unsigned char* pixels;
};

#endif // CustomZBuffer_H
