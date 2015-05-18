#ifndef RENDER_PASS_MODEL_H
#define RENDER_PASS_MODEL_H

#include "RenderPass.h"
#include "VertexArrayObject.h"
#include "FrameBufferObject.h"
#include "ShaderTools/VertexArrayObjects/Mesh.h"


class RenderPassModel: public RenderPassBase
{
   public: 
    RenderPassModel(std::vector<VertexArrayObject*> meshes, ShaderProgram* shaderProgram);
	RenderPassModel(std::vector<VertexArrayObject*> meshes, ShaderProgram* shaderProgram, int width, int height);
	RenderPassModel(Mesh* mesh, ShaderProgram* shaderProgram);
	RenderPassModel(Mesh* mesh, ShaderProgram* shaderProgram, int width, int height);
	RenderPassModel* setFrameBufferObject(FrameBufferObject* frameBufferObject);

	void run();
	void runOneMesh();



		template <class T>
	RenderPassModel* update(std::string name, T value) {
	  shaderProgram->update(name, value);
	return this;
	}


    protected:
    std::vector<VertexArrayObject*> meshes;
    Mesh* mesh;
};

#endif // RENDER_PASS_MODEL_H