#include "ShaderTools/OculusRenderer.h"
//#include "ShaderTools/Renderer.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"
#include <typeinfo>

using namespace std;
using namespace glm;

int main(int argc, char *argv[]) {
	int width, height;

	GLFWwindow* window = generateWindow();


	Cube* cube = new Cube(vec3(2.0f, 2.0f, -10.0f), 1.0f);
	std::vector<std::string> attachShaders = { "/Test_Telepresence/phong.vert", "/Test_Telepresence/phong.frag" };
	RenderPass* cubePass = new RenderPass(
		cube,
		new ShaderProgram(attachShaders));

	cubePass
		->update("diffuseColor", vec3(1.0, 0.0, 0.0))
		->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);

	vec3 lightPos = vec3(2.0f, 2.0f, 2.0f);

	int count = 0;
	render(window, [&](float deltaTime, glm::mat4 projection, glm::mat4 view) {
		//cout << count++ << " " << deltaTime << endl;

		glfwGetWindowSize(window, &width, &height);

		cubePass
			->update("modelMatrix", mat4(1.0f))
			->update("projectionMatrix", projection)
			->update("viewMatrix", view)
			->update("lightPosition", lightPos)
			->run();
	},
		[&]()
	{

		//performHHF();
	});

	return 0;
}
