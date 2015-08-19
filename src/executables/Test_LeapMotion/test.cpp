#include "ShaderTools/Renderer.h"
#include "ShaderTools/RenderPass.h"
#include "Compression/TextureTools.h"
#include "ShaderTools/VertexArrayObjects/Sphere.h"
#include "LeapMotionHandler.h"
#include <typeinfo>

using namespace std;
using namespace glm;

int main(int argc, char *argv[]) {

	int width = 1280;
	int height = 720;

	GLFWwindow* window = generateWindow(width, height, 100, 100);

	// Create a sample listener and controller
	LeapMotionHandler leapHandler;
	Controller controller;

	// Leap Constraint
	if (argc > 1 && strcmp(argv[1], "--bg") == 0)
		controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);

	std::vector<std::string> attachShaders = { "/Test_Telepresence/phong.vert", "/Test_Telepresence/phong.frag" };
	Sphere* sphere = new Sphere(10.0f);
	RenderPass* spherePass = new RenderPass(
		sphere,
		new ShaderProgram(attachShaders));

	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 150.0f, 300.0f), glm::vec3(0.0f, 150.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projection = glm::perspective(60.0f, (float)width / height, 0.1f, 1000.0f);
	vec3 lightPos = vec3(2.0f, 2.0f, 2.0f);

	int count = 0;
	render(window, [&](float deltaTime) {
		//cout << count++ << " " << deltaTime << endl;

		glfwGetWindowSize(window, &width, &height);

		spherePass
			->clear(1.0, 1.0, 1.0, 1.0)
			->update("lightPosition", lightPos)
			->update("diffuseColor", vec3(0.2, 0.2, 0.2))
			->update("projectionMatrix", projection)
			->update("viewMatrix", view);

		vector<Bone> bones = leapHandler.getBoneList(controller);
		InteractionBox box = controller.frame().interactionBox();

		//draw Bones
		if (bones.size() != 0){
			for (int i = 0; i < bones.size(); i++)
			{
				//compute rotation and translation of Leap Motion Bones
				mat4 rotationMat = leapHandler.convertLeapMatToGlm(bones[i].basis());
				mat4 translateMat = translate(mat4(1.0f), leapHandler.convertLeapVecToGlm(bones[i].nextJoint()));
				mat4 leapWorldCoordinates = translateMat * rotationMat;

				//draw Bone 
				spherePass
					->update("modelMatrix", leapWorldCoordinates)
					->run();
			}

			//additional wrist bone (first hand)
			mat4 rotationMatFirstHand = leapHandler.convertLeapMatToGlm(bones[16].basis());
			mat4 translateMatFirstHand = translate(mat4(1.0f), leapHandler.convertLeapVecToGlm(bones[16].prevJoint()));
			mat4 finalMatFirstHand = translateMatFirstHand * rotationMatFirstHand;

			spherePass
				->update("modelMatrix", finalMatFirstHand)
				->run();

			
			//additional wrist bone (second hand)
			if (bones.size() > 20){
				mat4 rotationMatSecondHand = leapHandler.convertLeapMatToGlm(bones[36].basis());
				mat4 translateMatSecondHand = translate(mat4(1.0f), leapHandler.convertLeapVecToGlm(bones[36].prevJoint()));
				mat4 finalMatSecondHand = translateMatSecondHand * rotationMatSecondHand;

				spherePass
					->update("modelMatrix", finalMatSecondHand)
					->run();
			}
		}
	}); 
	
	return 0;
}
