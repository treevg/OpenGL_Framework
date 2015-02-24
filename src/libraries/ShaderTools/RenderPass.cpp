#include "RenderPass.h"


RenderPass::RenderPass(VertexArrayObject* vertexArrayObject, ShaderProgram* shaderProgram)
	: vertexArrayObject(vertexArrayObject), shaderProgram(shaderProgram), frameBufferObject(0)
{
	frameBufferObject = new FrameBufferObject();
}

RenderPass::RenderPass(std::vector<Mesh*> meshes, ShaderProgram* shaderProgram)
	: meshes(meshes), shaderProgram(shaderProgram), frameBufferObject(0)
{
	frameBufferObject = new FrameBufferObject();
}

RenderPass::RenderPass(Mesh* mesh, ShaderProgram* shaderProgram)
	: mesh(mesh), shaderProgram(shaderProgram), frameBufferObject(0)
{
	frameBufferObject = new FrameBufferObject();
}


RenderPass::RenderPass(VertexArrayObject* vertexArrayObject, ShaderProgram* shaderProgram, int width, int height)
	: vertexArrayObject(vertexArrayObject), shaderProgram(shaderProgram), frameBufferObject(0)
{
	autoGenerateFrameBufferObject(width, height);
}

RenderPass::RenderPass(VertexArrayObject* vertexArrayObject, ShaderProgram* shaderProgram, FrameBufferObject* frameBufferObject)
	: vertexArrayObject(vertexArrayObject), shaderProgram(shaderProgram), frameBufferObject(frameBufferObject)
{
}



void RenderPass::run() {
	frameBufferObject->bind();
	shaderProgram->use();
	vertexArrayObject->draw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass::runOneMesh() {
	cout << "drawing mesh" << endl;
	frameBufferObject->bind();
	shaderProgram->use();
	mesh->draw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



void RenderPass::runMeshes() {
      frameBufferObject->bind();

   //  cout << "DEBUG: meshes count "<< this->meshes.size() << endl;

	for (int i=0; i<this->meshes.size(); i++ ){

	    vertexArrayObject = meshes[i];
        shaderProgram->use();
        vertexArrayObject->draw();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	
}

void RenderPass::autoGenerateFrameBufferObject(int width, int height) {
	if (frameBufferObject) delete frameBufferObject;
	frameBufferObject = new FrameBufferObject(&(shaderProgram->outputMap), width, height);
}

RenderPass* RenderPass::texture(std::string name, GLuint textureHandle) {
	shaderProgram->texture(name, textureHandle);
	return this;
}

RenderPass* RenderPass::texture(std::string name, std::vector<GLuint> textureHandle) {
	for (auto i: textureHandle ){

     shaderProgram->texture(name, i);

	}
	
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

GLuint RenderPass::get(std::string name) {
	return frameBufferObject->get(name);
}

