#include "FrameBufferObject.h"

FrameBufferObject::FrameBufferObject() {
	frameBufferObjectHandle = 0;
}

FrameBufferObject::FrameBufferObject(std::map<std::string, ShaderProgram::Info>* outputMap, int width, int height) {
	int size = outputMap->size();

	glGenFramebuffers(1, &frameBufferObjectHandle);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObjectHandle);
    // drawBuffers.resize(size);

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
	    // drawBuffers[e.second.location] = currentAttachment;
    }

    // glDrawBuffers(size, &drawBuffers[0]);

    //TODO how to acces colorTexture from outside of this fbo???

    // glGenTextures(1, &colorAttachment);
    // glBindTexture( GL_TEXTURE_2D, colorAttachment);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment, 0);

	GLuint depthTexture;
	glGenTextures( 1, &depthTexture);
	glBindTexture( GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, width, height, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
}

void FrameBufferObject::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObjectHandle);
	// glDrawBuffers(drawBuffers.size(), &drawBuffers[0]);
}

FrameBufferObject* FrameBufferObject::clear(float r, float g, float b, float a) {
	bind();
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return this;
}

FrameBufferObject* FrameBufferObject::clear() {
	bind();
	clear(0, 0, 0, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return this;
}

FrameBufferObject* FrameBufferObject::clearDepth() {
	bind();
	glClear(GL_DEPTH_BUFFER_BIT);
	return this;
}

GLuint FrameBufferObject::get(std::string name) {
	return textureMap[name];
}


GLuint FrameBufferObject::getFrameBufferObjectHandle() const {
	return frameBufferObjectHandle;
}

void FrameBufferObject::setFrameBufferObjectHandle(
		GLuint frameBufferObjectHandle) {
	this->frameBufferObjectHandle = frameBufferObjectHandle;
}

GLuint FrameBufferObject::getHandle(){
	return frameBufferObjectHandle;
}

