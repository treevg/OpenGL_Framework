#ifndef RENDER_PASS_H
#define RENDER_PASS_H

#include "ShaderProgram.h"
#include "VertexArrayObject.h"

class RenderPass
{
public:
	RenderPass(VertexArrayObject* vao, ShaderProgram* sp);
	void run();

	template <class T>
	RenderPass* update(std::string name, T value) {
		sp->update(name, value);
		return this;
	}
protected:
	ShaderProgram* sp;
	VertexArrayObject* vao;
};

#endif // RENDER_PASS_H
