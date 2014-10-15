#include "RenderPass.h"

RenderPass::RenderPass(VertexArrayObject* vao, ShaderProgram* sp)
	: vao(vao), sp(sp)
{
}

void RenderPass::run() {
	sp->use();
	vao->draw();
}
