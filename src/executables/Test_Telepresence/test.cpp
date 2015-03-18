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

	static const int DepthWidth = 512;
	static const int DepthHeight = 424;
	
	int width = 1280;
	int height = 720;
	GLFWwindow* window = generateWindow();

	// Create a sample listener and controller
	LeapMotionHandler leapHandler;
	Controller controller;

	//Initialize Kinect
	KinectHandler kinectHandler;
	kinectHandler.InitializeDefaultSensor();



	GLfloat *tex;
	//tex = (float*)malloc(DepthWidth * DepthHeight * 3 * sizeof(float));
	tex = new float[DepthWidth * DepthHeight * 3 * sizeof(float)];


	if (argc > 1 && strcmp(argv[1], "--bg") == 0)
		controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);



	Cube* cube = new Cube(vec3(2.0, 2.0, -7), 2.0f);
	std::vector<std::string> attachShaders = { "/Test_Telepresence/v.vert", "/Test_Telepresence/f.frag" };
	RenderPass* cubePass = new RenderPass(
		cube,
		new ShaderProgram(attachShaders));
	
	Sphere* sphere = new Sphere(0.1);
	RenderPass* spherePass = new RenderPass(
		sphere,
		new ShaderProgram(attachShaders));
	
	std::vector<std::string> depthShaders = { "/Test_Telepresence/fullscreen.vert", "/Test_Telepresence/simpleTexture.frag" };
	Quad* quad = new Quad();
	RenderPass* kinectPass = new RenderPass(
		quad,
		new ShaderProgram(depthShaders));

	//kinectPass
	//	->update("resolution", getResolution(window));
		


	//create new texture
	GLuint depthTex;
	glGenTextures(1, &depthTex);

	//bind the texture
	glBindTexture(GL_TEXTURE_2D, depthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, DepthWidth, DepthHeight, 0, GL_RGB, GL_FLOAT, tex);

	//texture settings
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, true);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);



	// camera parameters
	float glnear = 0.1f;
	float glfar = 100.0f;
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projection = perspective(60.0f * PI / 180.0f, (float)width / height, glnear, glfar);
	mat4 modelMatrixJoints = mat4(1.0f);
	mat4 modelMatrixGoal = mat4(1.0f);
	vec3 lightPos = vec3(2, 2, -2);
	


	//cubePass->update("lightPosition", lightPos)
	//	->update("viewMatrix", view)
	//	->update("diffuseColor", vec3(1.0, 0.0, 0.0))
	//	->update("projectionMatrix", projection);

	//spherePass->update("lightPosition", lightPos)
	//	->update("viewMatrix", view)
	//	->update("projectionMatrix", projection);

	

	int count = 0;

	render(window, [&](float deltaTime) {
		//cout << count++ << " " << deltaTime << endl;
		glfwGetWindowSize(window, &width, &height);
		projection = perspective(60.0f * PI / 180.0f, (float)width / height, glnear, glfar);

		tex = new float[DepthWidth * DepthHeight * 3 * sizeof(float)];
		kinectHandler.Update(tex);


		//bind the texture
		glBindTexture(GL_TEXTURE_2D, depthTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, DepthWidth, DepthHeight, 0, GL_RGB, GL_FLOAT, tex);

		//texture settings
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, true);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);



		vector<Bone> bones = leapHandler.getBoneList(controller);
		InteractionBox box = controller.frame().interactionBox();

		////draw Goal
		//cubePass
		//	->clear(0.9, 0.9, 0.9, 0)
		//	->update("modelMatrix", modelMatrixGoal)
		//	->run();

		////draw Bones
		//if (bones.size() == 0)
		//	spherePass
		//		->update("modelMatrix", modelMatrixJoints)
		//		->run();
		//else{
		//	for (int i = 0; i < bones.size(); i++)
		//	{
		//		modelMatrixJoints = mat4(1.0f);

		//		//compute rotation and translation 
		//		mat4 rotationMat = leapHandler.convertLeapMatToGlm(bones[i].basis());
		//		mat4 translateMat = translate(modelMatrixJoints, leapHandler.convertLeapVecToGlm(box.normalizePoint(bones[i].nextJoint())) * 2.0f - 1.0f);
		//		mat4 finalMat = translateMat * rotationMat;

		//		//draw Bone 
		//		spherePass
		//			->update("modelMatrix", finalMat)
		//			->run();
		//	}

		//	//compute direction and translation for pointing bone
		//	modelMatrixJoints = mat4(1.0f);
		//	vec3 eins = leapHandler.convertLeapVecToGlm(box.normalizePoint(bones[7].nextJoint())) * 2.0f - 1.0f;
		//	vec3 zwei = leapHandler.convertLeapVecToGlm(box.normalizePoint(bones[5].prevJoint())) * 2.0f - 1.0f;
		//	vec3 dTest = eins - zwei; 

		//	

		//	vec3 directionTest1 = dTest;
		//	mat4 boneTestRot = leapHandler.convertLeapMatToGlm(bones[7].basis());
		//	mat4 boneTest = translate(modelMatrixJoints, leapHandler.convertLeapVecToGlm(box.normalizePoint(bones[7].nextJoint())) * 2.0f - 1.0f);
		//	mat4 dirMatTest = boneTest; //* boneTestRot;

		//	//show pointing direction 
		//	spherePass
		//		->update("modelMatrix", translate(dirMatTest, directionTest1 * 1.0f))
		//		->run();
		//	spherePass
		//		->update("modelMatrix", translate(dirMatTest, directionTest1 * 1.8f))
		//		->run();
		//	spherePass
		//		->update("modelMatrix", translate(dirMatTest, directionTest1 * 2.6f))
		//		->run();
		//	spherePass
		//		->update("modelMatrix", translate(dirMatTest, directionTest1 * 3.4f))
		//		->run();
		//	spherePass
		//		->update("modelMatrix", translate(dirMatTest, directionTest1 * 4.2f))
		//		->run();
		//	spherePass
		//		->update("modelMatrix", translate(dirMatTest, directionTest1 * 5.0f))
		//		->run();
		//	spherePass
		//		->update("modelMatrix", translate(dirMatTest, directionTest1 * 5.8f))
		//		->run();
		//	spherePass
		//		->update("modelMatrix", translate(dirMatTest, directionTest1 * 6.6f))
		//		->run();
		//	spherePass
		//		->update("modelMatrix", translate(dirMatTest, directionTest1 * 7.4f))
		//		->run();
		//	spherePass
		//		->update("modelMatrix", translate(dirMatTest, directionTest1 * 8.2f))
		//		->run();
		//	spherePass
		//		->update("modelMatrix", translate(dirMatTest, directionTest1 * 9.0f))
		//		->run();
		//	spherePass
		//		->update("modelMatrix", translate(dirMatTest, directionTest1 * 9.8f))
		//		->run();
		//	spherePass
		//		->update("modelMatrix", translate(dirMatTest, directionTest1 * 10.6f))
		//		->run();
		//	spherePass
		//		->update("modelMatrix", translate(dirMatTest, directionTest1 * 11.4f))
		//		->run();
		//	spherePass
		//		->update("modelMatrix", translate(dirMatTest, directionTest1 * 12.2f))
		//		->run();



		//	//check for intersection
		//	if (bones.size() != 0){
		//		bool a = leapHandler.checkForIntersection(cube->getVertices(), leapHandler.convertLeapVecToGlm(box.normalizePoint(bones[7].nextJoint())) * 2.0f - 1.0f, directionTest1);
		//		if (a == true){
		//			//cout << "Getroffen" << endl;
		//			cubePass
		//				->update("diffuseColor", vec3(0.0, 1.0, 0.0))
		//				->run();
		//		}
		//		else{
		//			//cout << "Nicht getroffen" << endl;
		//			cubePass
		//				->update("diffuseColor", vec3(1.0, 0.0, 0.0))
		//				->run();
		//		}
		//	}


		//	//Hack fuer zusaetzliche Bobbels in linker und rechter Hand 
		//	modelMatrixJoints = mat4(1.0f);

		//	mat4 rotationMatFirstHand = leapHandler.convertLeapMatToGlm(bones[16].basis());
		//	mat4 translateMatFirstHand = translate(modelMatrixJoints, leapHandler.convertLeapVecToGlm(box.normalizePoint(bones[16].prevJoint())) * 2.0f - 1.0f);
		//	mat4 finalMatFirstHand = translateMatFirstHand * rotationMatFirstHand;

		//	spherePass
		//		->update("modelMatrix", finalMatFirstHand)
		//		->run();

		//	if (bones.size() > 20){
		//		mat4 rotationMatSecondHand = leapHandler.convertLeapMatToGlm(bones[36].basis());
		//		mat4 translateMatSecondHand = translate(modelMatrixJoints, leapHandler.convertLeapVecToGlm(box.normalizePoint(bones[36].prevJoint())) * 2.0f - 1.0f);
		//		mat4 finalMatSecondHand = translateMatSecondHand * rotationMatSecondHand;
		//	spherePass
		//		->update("modelMatrix", finalMatSecondHand)
		//		->run();

		//	}
		//}

		kinectPass
			->clear(0.0, 0.0, 0.0, 1.0)
			->texture("tex", depthTex)
			//->texture("tex", TextureTools::loadTexture("D:/FP14/FP14_OpenGL_Framework/resources/bambus.jpg"))
			->run();

		delete(tex);

	}); 
	
	return 0;
}
