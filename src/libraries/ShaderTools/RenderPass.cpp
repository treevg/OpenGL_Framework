#include "RenderPass.h"


RenderPass::RenderPass(VertexArrayObject* vertexArrayObject, ShaderProgram* shaderProgram)
	: RenderPassBase(vertexArrayObject, shaderProgram)
{
	
}


RenderPass::RenderPass(VertexArrayObject* vertexArrayObject, ShaderProgram* shaderProgram, int width, int height)
	: RenderPassBase(vertexArrayObject, shaderProgram, width, height)
	{
	    
     }

RenderPass::RenderPass(VertexArrayObject* vertexArrayObject, ShaderProgram* shaderProgram, FrameBufferObject* frameBufferObject)
	: RenderPassBase( vertexArrayObject, shaderProgram, frameBufferObject)
{
}



void RenderPass::run() {
	frameBufferObject->bind();
	shaderProgram->use();
	vertexArrayObject->draw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



