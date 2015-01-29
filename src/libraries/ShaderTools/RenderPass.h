#ifndef RENDER_PASS_H
#define RENDER_PASS_H

#include "VertexArrayObject.h"
#include "FrameBufferObject.h"

class RenderPass
{
public:
	RenderPass(VertexArrayObject* vertexArrayObject, ShaderProgram* shaderProgram);
	RenderPass(VertexArrayObject* vertexArrayObject, ShaderProgram* shaderProgram, int width, int height);
	RenderPass(VertexArrayObject* vertexArrayObject, ShaderProgram* shaderProgram, FrameBufferObject* frameBufferObject);
	RenderPass* run();
	RenderPass* runInFBO();
	void autoGenerateFrameBufferObject(int width, int height);
	GLuint get(std::string name);

	RenderPass* clear(float r, float g, float b, float a);
	RenderPass* texture(std::string name, GLuint textureID);
	RenderPass* texture(std::string name, GLuint textureID, GLuint samplerID);

	template <class T>
	RenderPass* update(std::string name, T value) {
		shaderProgram->update(name, value);
		return this;
	}

	ShaderProgram* shaderProgram;
	VertexArrayObject* vertexArrayObject;
	FrameBufferObject* frameBufferObject;
protected:
};

#endif // RENDER_PASS_H
