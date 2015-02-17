#ifndef RENDER_PASS_H
#define RENDER_PASS_H

#include <vector>
#include "VertexArrayObject.h"
#include "FrameBufferObject.h"
#include "ShaderTools/VertexArrayObjects/Mesh.h"

class RenderPass
{
public:
	RenderPass(VertexArrayObject* vertexArrayObject, ShaderProgram* shaderProgram);
	RenderPass(std::vector<Mesh*> meshes, ShaderProgram* shaderProgram);
	RenderPass(Mesh* mesh, ShaderProgram* shaderProgram);
	RenderPass(VertexArrayObject* vertexArrayObject, ShaderProgram* shaderProgram, int width, int height);
	RenderPass(VertexArrayObject* vertexArrayObject, ShaderProgram* shaderProgram, FrameBufferObject* frameBufferObject);
	void run();
	void runMeshes();
	void runOneMesh();
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
    std::vector<Mesh*> meshes;
    Mesh* mesh;
	
protected:
};

#endif // RENDER_PASS_H
