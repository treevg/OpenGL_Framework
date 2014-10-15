#ifndef RENDER_PASS_H
#define RENDER_PASS_H

#include "VertexArrayObject.h"
#include "FrameBufferObject.h"

class RenderPass
{
public:
	RenderPass(VertexArrayObject* vao, ShaderProgram* sp);
	RenderPass(VertexArrayObject* vao, ShaderProgram* sp, FrameBufferObject* fbo);
	void run();
	void autoGenerateFrameBufferObject(int width, int height);

	template <class T>
	RenderPass* update(std::string name, T value) {
		sp->update(name, value);
		return this;
	}
protected:
	ShaderProgram* sp;
	VertexArrayObject* vao;
	FrameBufferObject* fbo;
};

#endif // RENDER_PASS_H
