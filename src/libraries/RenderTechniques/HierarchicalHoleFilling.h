#ifndef HIERARCHICAL_HOLE_FILLING_H
#define HIERARCHICAL_HOLE_FILLING_H

#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"

class HierarchicalHoleFilling
{
public:
	HierarchicalHoleFilling(
		glm::vec2 resolution, 
		VertexArrayObject* vertexArrayObject = NULL, 
		int mipmapNumber = INT_MAX,
		std::string vertexShaderPath = "/Filters/fullscreen.vert",
		std::string fillFragmentPath = "/RenderTechniques/hhfFill.frag",
		std::string reduceFragmentShaderPath = "/RenderTechniques/hhfReduce.frag");
	~HierarchicalHoleFilling();
	
	HierarchicalHoleFilling* run();
	HierarchicalHoleFilling* texture(GLuint input);
	GLuint getOutput();
	int getMipmapNumber();

private:
	RenderPassBase* reduce;
	RenderPassBase* fill;
	VertexArrayObject* vertexArrayObject;

	GLuint mipmapNumber;
	GLuint mipmap;
	GLuint inputTexture;
	std::vector<FrameBufferObject*> mipmapFBOs;
};

#endif // HIERARCHICAL_HOLE_FILLING_H
