#include "RenderPass.h"

RenderPass::RenderPass(ShaderProgram* shaderProgram)
	: shaderProgram(shaderProgram), frameBufferObject(0), vertexArrayObject(NULL)
{
	frameBufferObject = new FrameBufferObject();
}

RenderPass::RenderPass(ShaderProgram* shaderProgram, FrameBufferObject* frameBufferObject)
: vertexArrayObject(NULL), shaderProgram(shaderProgram), frameBufferObject(frameBufferObject), zTest(true)
{
}

RenderPass::RenderPass(ShaderProgram* shaderProgram, int width, int height)
: vertexArrayObject(NULL), shaderProgram(shaderProgram), frameBufferObject(0), zTest(true)
{
	autoGenerateFrameBufferObject(width, height);
}

RenderPass::RenderPass(VertexArrayObject* vertexArrayObject, ShaderProgram* shaderProgram)
	: vertexArrayObject(vertexArrayObject), shaderProgram(shaderProgram), frameBufferObject(0), zTest(true)
{
	frameBufferObject = new FrameBufferObject();
}

RenderPass::RenderPass(VertexArrayObject* vertexArrayObject, ShaderProgram* shaderProgram, int width, int height)
	: vertexArrayObject(vertexArrayObject), shaderProgram(shaderProgram), frameBufferObject(0), zTest(true)
{
	autoGenerateFrameBufferObject(width, height);
}

RenderPass::RenderPass(VertexArrayObject* vertexArrayObject, ShaderProgram* shaderProgram, FrameBufferObject* frameBufferObject)
	: vertexArrayObject(vertexArrayObject), shaderProgram(shaderProgram), frameBufferObject(frameBufferObject), zTest(true)
{
}

void RenderPass::preDraw() {
	zTest? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	frameBufferObject->bind();
	shaderProgram->use();
}

void RenderPass::postDraw() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

RenderPass* RenderPass::run() {
	preDraw();
	vertexArrayObject->draw();
	postDraw();
	return this;
}

RenderPass* RenderPass::run(VertexArrayObject* vao) {
	preDraw();
	vao->draw();
	postDraw();
	return this;
}

RenderPass* RenderPass::run(std::vector<VertexArrayObject*> renderQueue) {
	preDraw();
	for (auto e : renderQueue) {
		e->draw();
	}
	postDraw();
	return this;
}

void RenderPass::autoGenerateFrameBufferObject(int width, int height) {
	if (frameBufferObject) delete frameBufferObject;
	frameBufferObject = new FrameBufferObject(&(shaderProgram->outputMap), width, height);
}

RenderPass* RenderPass::depthTest(bool toggle) {
	zTest = toggle;
	return this;
}

RenderPass* RenderPass::texture(std::string name, GLuint textureHandle) {
    shaderProgram->texture(name, textureHandle);
	return this;
}

RenderPass* RenderPass::texture(std::string name, Texture* texture) {
    shaderProgram->texture(name, texture->getHandle());
	return this;
}

RenderPass* RenderPass::texture(std::string name, GLuint textureHandle, GLuint samplerHandle) {
	shaderProgram->texture(name, textureHandle, samplerHandle);
	return this;
}

RenderPass* RenderPass::clear(float r, float g, float b, float a) {
	frameBufferObject->clear(r, g, b, a);
	return this;
}

RenderPass* RenderPass::clear() {
	frameBufferObject->clear();
	return this;
}

RenderPass* RenderPass::clearDepth() {
	frameBufferObject->clearDepth();
	return this;
}

GLuint RenderPass::get(std::string name) {
	return frameBufferObject->get(name);
}

ShaderProgram* RenderPass::getShaderProgram() {
	return shaderProgram;
}

VertexArrayObject* RenderPass::getVertexArrayObject() {
	return vertexArrayObject;
}

FrameBufferObject* RenderPass::getFrameBufferObject() {
	return frameBufferObject;
}

RenderPass* RenderPass::setShaderProgram(ShaderProgram* shaderProgram) {
	this->shaderProgram = shaderProgram;
	return this;
}

RenderPass* RenderPass::setVertexArrayObject(VertexArrayObject* vertexArrayObject) {
	this->vertexArrayObject = vertexArrayObject;
	return this;
}

RenderPass* RenderPass::setFrameBufferObject(FrameBufferObject* frameBufferObject) {
	this->frameBufferObject = frameBufferObject;
	return this;
}
