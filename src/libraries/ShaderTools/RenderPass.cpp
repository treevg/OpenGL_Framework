#include "RenderPass.h"

RenderPass::RenderPass(VertexArrayObject* vao, ShaderProgram* sp)
	: vao(vao), sp(sp)
{
	fbo = new FrameBufferObject();
}

RenderPass::RenderPass(VertexArrayObject* vao, ShaderProgram* sp, FrameBufferObject* fbo)
	: vao(vao), sp(sp), fbo(fbo)
{
}

void RenderPass::run() {
	fbo->bind();
	sp->use();
	vao->draw();
}

void RenderPass::autoGenerateFrameBufferObject(int width, int height) {
	delete fbo;
	fbo = new FrameBufferObject(&(sp->outputMap), width, height);
}