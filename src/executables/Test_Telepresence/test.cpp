#include "ShaderTools/OculusRenderer.h"
#include "ShaderTools/RenderPass.h"
#include "Compression/TextureTools.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"
#include "ShaderTools/VertexArrayObjects/Sphere.h"
#include "LeapMotionHandler.h"
#include "KinectHandler.h"
#include <typeinfo>

using namespace std;
using namespace glm;

int main(int argc, char *argv[]) {

	int width = 1280;
	int height = 720;
	GLFWwindow* window = generateWindow();

	Cube* cube = new Cube(glm::vec3(0.0, -0.6, 0.0), 0.5f);
	std::vector<std::string> attachShaders = { "/Test_Telepresence/v.vert", "/Test_Telepresence/f.frag" };
	RenderPass* cubePass = new RenderPass(
		cube,
		new ShaderProgram(attachShaders));

	Sphere* sphere = new Sphere(0.3f);
	RenderPass* spherePass = new RenderPass(
		sphere,
		new ShaderProgram(attachShaders));

	cubePass
		->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);

	spherePass
		->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);

	glm::vec3 lightPos = glm::vec3(2.0f, 2.0f, 2.0f);

	int count = 0;

	render(window, [&](float deltaTime, glm::mat4 projection, glm::mat4 view) {
		//cout << count++ << " " << deltaTime << endl;
		glfwGetWindowSize(window, &width, &height);

		cubePass
			->update("lightPosition", lightPos)
			->update("modelMatrix", glm::mat4(1.0))
			->update("viewMatrix", view)
			->update("projectionMatrix", projection)
			->update("diffuseColor", glm::vec3(1.0, 1.0, 0.0))
			->run();

		spherePass
			->update("lightPosition", lightPos)
			->update("modelMatrix", glm::mat4(1.0))
			->update("viewMatrix", view)
			->update("projectionMatrix", projection)
			->update("diffuseColor", glm::vec3(0.0, 0.0, 1.0))
			->run();
	});

	return 0;
}

/*
	// Create a sample listener and controller
	LeapMotionHandler leapHandler;
	Controller controller;



	if (argc > 1 && strcmp(argv[1], "--bg") == 0)
		controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);



	Cube* cube = new Cube(vec3(2.0, 2.0, -7), 2.0f);
	std::vector<std::string> attachShaders = { "/Test_Telepresence/v.vert", "/Test_Telepresence/f.frag" };
	RenderPass* cubePass = new RenderPass(
		cube,
		new ShaderProgram(attachShaders));
	


	// camera parameters
	float glnear = 0.1f;
	float glfar = 100.0f;
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projection = perspective(60.0f * PI / 180.0f, (float)width / height, glnear, glfar);
	mat4 modelMatrixJoints = mat4(1.0f);
	mat4 modelMatrixGoal = mat4(1.0f);
	vec3 lightPos = vec3(2, 2, -2);
	


	cubePass->update("lightPosition", lightPos)
		->update("viewMatrix", view)
		->update("diffuseColor", vec3(1.0, 0.0, 0.0))
		->update("projectionMatrix", projection);

	

	int count = 0;

	render(window, [&](float deltaTime) {
		//cout << count++ << " " << deltaTime << endl;
		glfwGetWindowSize(window, &width, &height);
		projection = perspective(60.0f * PI / 180.0f, (float)width / height, glnear, glfar);

		vector<Bone> bones = leapHandler.getBoneList(controller);
		InteractionBox box = controller.frame().interactionBox();

		//draw Goal
		cubePass
			->clear(0.9, 0.9, 0.9, 0)
			->update("modelMatrix", modelMatrixGoal)
			->run();
	}); 
	
	return 0;
}
*/
