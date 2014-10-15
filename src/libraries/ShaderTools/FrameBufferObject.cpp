#include "FrameBufferObject.h"

FrameBufferObject::FrameBufferObject() {
	frameBufferObjectHandle = 0;
}

FrameBufferObject::FrameBufferObject(std::map<std::string, ShaderProgram::Info>* outputMap, int width, int height) {
	int size = outputMap->size();

	glGenFramebuffers(1, &frameBufferObjectHandle);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObjectHandle);
    std::vector<GLuint> drawBufferHandles(size);

    for (auto e : *outputMap) {
    	GLuint handle;
    	glGenTextures(1, &handle);
	    glBindTexture(GL_TEXTURE_2D, handle);
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	    GLuint currentAttachment = GL_COLOR_ATTACHMENT0 + e.second.location;
	    glFramebufferTexture2D(GL_FRAMEBUFFER, currentAttachment, GL_TEXTURE_2D, handle, 0);

    	textureMap[e.first] = handle;
	    drawBufferHandles[e.second.location] = currentAttachment;
    }
    glDrawBuffers(size, &drawBufferHandles[0]);
}

void FrameBufferObject::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObjectHandle);
}