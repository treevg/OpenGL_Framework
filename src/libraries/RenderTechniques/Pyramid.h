#include "ShaderTools/VertexArrayObject.h"
#include "ShaderTools/FrameBufferObject.h"

class Pyramid
{
public:
	Pyramid(int width, int height, std::string pullFragmentShaderPath, std::string pushFragmentShaderPath = "");
	
	~Pyramid();

	Pyramid* pull();
	Pyramid* push();
	Pyramid* run();

	Pyramid* setLimit(int limit);
	Pyramid* clear(float r, float g, float b, float a, int level = -1);
	Pyramid* clear(int level = -1);
	Pyramid* texture(std::string name, GLuint textureID);
	GLuint get(std::string name);
	int getMipmapNumber();

	template <class T>
	Pyramid* update(std::string name, T value) {
		pullShaderProgram->update(name, value);
		if (pushShaderProgram != NULL) {
			pushShaderProgram->update(name, value);
		}
		return this;
	}

private:
	int limit;

	std::map<std::string, GLuint> textureMap;

	// Shader programs to use
	ShaderProgram* pullShaderProgram;
	ShaderProgram* pushShaderProgram;

	// Vertex array object to render 
	VertexArrayObject* vertexArrayObject;

	// mip framebuffer objects to render to
	std::vector<GLuint> fboHandles;
};