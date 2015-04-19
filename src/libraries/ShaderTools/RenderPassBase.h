#ifndef RENDER_PASS_BASE_H
#define RENDER_PASS_BASE_H

#include <vector>
#include "VertexArrayObject.h"
#include "FrameBufferObject.h"


class RenderPassBase
{
public:
	RenderPassBase(VertexArrayObject* vertexArrayObject, ShaderProgram* shaderProgram);
	RenderPassBase(ShaderProgram* shaderProgram);
	RenderPassBase(ShaderProgram* shaderProgram, int width, int height);

	RenderPassBase(VertexArrayObject* vertexArrayObject, ShaderProgram* shaderProgram, int width, int height);
	RenderPassBase(VertexArrayObject* vertexArrayObject, ShaderProgram* shaderProgram, FrameBufferObject* frameBufferObject);
	
        virtual void run() = 0;
	
	void autoGenerateFrameBufferObject(int width, int height);
	
	GLuint get(std::string name);

	RenderPassBase* clear(float r, float g, float b, float a);
	RenderPassBase* clear();
	RenderPassBase* clearDepth();
	RenderPassBase* texture(std::string name, GLuint textureID);
	RenderPassBase* texture(std::string name, GLuint textureID, GLuint samplerID);
	

	RenderPassBase* setFrameBufferObject(FrameBufferObject* frameBufferObject);
	FrameBufferObject* getFrameBufferObject();

	template <class T>
	RenderPassBase* update(std::string name, T value) {
	  shaderProgram->update(name, value);
	return this;
	}

	ShaderProgram* shaderProgram;
	VertexArrayObject* vertexArrayObject;
	FrameBufferObject* frameBufferObject;
   
	
protected:
};

#endif // RENDER_PASS_BASE_H
