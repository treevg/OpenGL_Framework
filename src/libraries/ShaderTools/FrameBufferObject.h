#ifndef FRAME_BUFFER_OBJECT_H
#define FRAME_BUFFER_OBJECT_H

#include "ShaderProgram.h"

class FrameBufferObject
{
public:
	FrameBufferObject();
	FrameBufferObject(std::map<std::string, ShaderProgram::Info>* outputMap, int width, int height);
	void bind();
protected:
	GLuint frameBufferObjectHandle;
	std::map<std::string, GLuint> textureMap;
};

#endif // FRAME_BUFFER_OBJECT_H