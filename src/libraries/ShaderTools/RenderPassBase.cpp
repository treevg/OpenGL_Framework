#include "RenderPassBase.h"


RenderPassBase::RenderPassBase(VertexArrayObject* vertexArrayObject, ShaderProgram* shaderProgram)
	: vertexArrayObject(vertexArrayObject), shaderProgram(shaderProgram), frameBufferObject(0)
{
	frameBufferObject = new FrameBufferObject();
}

RenderPassBase::RenderPassBase(ShaderProgram* shaderProgram)
	: shaderProgram(shaderProgram), frameBufferObject(0)
{
	frameBufferObject = new FrameBufferObject();
}


RenderPassBase::RenderPassBase(ShaderProgram* shaderProgram, int width, int height)
: shaderProgram(shaderProgram), frameBufferObject(0)
{

    autoGenerateFrameBufferObject(width, height);

}


RenderPassBase::RenderPassBase(VertexArrayObject* vertexArrayObject, ShaderProgram* shaderProgram, int width, int height)
	: vertexArrayObject(vertexArrayObject), shaderProgram(shaderProgram), frameBufferObject(0)
{
	autoGenerateFrameBufferObject(width, height);
}

RenderPassBase::RenderPassBase(VertexArrayObject* vertexArrayObject, ShaderProgram* shaderProgram, FrameBufferObject* frameBufferObject)
	: vertexArrayObject(vertexArrayObject), shaderProgram(shaderProgram), frameBufferObject(frameBufferObject)
{
}



void RenderPassBase::autoGenerateFrameBufferObject(int width, int height) {
	if (frameBufferObject) delete frameBufferObject;
	frameBufferObject = new FrameBufferObject(&(shaderProgram->outputMap), width, height);
}

RenderPassBase* RenderPassBase::texture(std::string name, GLuint textureHandle) {
	shaderProgram->texture(name, textureHandle);
	return this;
}



RenderPassBase* RenderPassBase::texture(std::string name, GLuint textureHandle, GLuint samplerHandle) {
	shaderProgram->texture(name, textureHandle, samplerHandle);
	return this;
}

RenderPassBase* RenderPassBase::clear(float r, float g, float b, float a) {
		frameBufferObject->clear(r, g, b, a);
		return this;
	}

GLuint RenderPassBase::get(std::string name) {
	return frameBufferObject->get(name);
}

RenderPassBase* RenderPassBase::clear() {
	frameBufferObject->clear();
	return this;
}

RenderPassBase* RenderPassBase::clearDepth() {
	frameBufferObject->clearDepth();
	return this;
}

RenderPassBase* RenderPassBase::setFrameBufferObject(FrameBufferObject* frameBufferObject) {
	this->frameBufferObject = frameBufferObject;
	return this;
}


FrameBufferObject* RenderPassBase::getFrameBufferObject() {
      return frameBufferObject;
}