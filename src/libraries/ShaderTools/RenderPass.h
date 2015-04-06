#ifndef RENDER_PASS_H
#define RENDER_PASS_H

#include <vector>
#include "VertexArrayObject.h"
#include "FrameBufferObject.h"
#include "RenderPassBase.h"


class RenderPass: public RenderPassBase
{
public:
	RenderPass(VertexArrayObject* vertexArrayObject, ShaderProgram* shaderProgram);
	RenderPass(VertexArrayObject* vertexArrayObject, ShaderProgram* shaderProgram, int width, int height);
	RenderPass(VertexArrayObject* vertexArrayObject, ShaderProgram* shaderProgram, FrameBufferObject* frameBufferObject);
	void run();


	


};

#endif // RENDER_PASS_H
