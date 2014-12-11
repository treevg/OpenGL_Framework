#include <ShaderTools/FrameBufferObject.h>
#include <ShaderTools/RenderPass.h>

/**
 * Owns RGBA 32bit Unsigned Integer textures as draw buffers
 */
class Slicemap : public FrameBufferObject
{
	public:
	Slicemap(std::map<std::string, ShaderProgram::Info>* outputMap, int width, int height);
};


/**
 * creates a Slicemap as FBO by default
 */
class SlicemapRenderPass : public RenderPass
{
	public:
	SlicemapRenderPass(VertexArrayObject* vertexArrayObject, ShaderProgram* shaderProgram, int width, int height);
	void autoGenerateSlicemap(int width, int height);
};

// create 32bit uint bitmask
GLuint createR32UIBitMask();
GLuint createRGBA32UIBitMask();
