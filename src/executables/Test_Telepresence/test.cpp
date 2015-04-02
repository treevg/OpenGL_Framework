#include "ShaderTools/OculusRenderer.h"
//#include "ShaderTools/Renderer.h"
#include "ShaderTools/RenderPass.h"
#include "Compression/TextureTools.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"
#include "ShaderTools/VertexArrayObjects/Sphere.h"
#include "PointCloud.h"
#include "LeapMotionHandler.h"
#include "KinectHandler.h"
#include <typeinfo>

using namespace std;
using namespace glm;

float leftRight = 0.0f;
float upDown = 0.0f;
float nearFar = 5.0f;



int main(int argc, char *argv[]) {

	static const int depthWidth = 512;
	static const int depthHeight = 424;
	
	int width = 720;
	int height = 720;

	GLFWwindow* window = generateWindow();

	// Create a sample listener and controller
	LeapMotionHandler leapHandler;
	Controller controller;

	// Initialize Kinect
	KinectHandler kinectHandler;
	kinectHandler.initializeDefaultSensor();

	GLfloat *colorData;
	GLfloat *positionData;
	//tex = (float*)malloc(depthWidth * depthHeight * 3 * sizeof(float)); // fuer Texturausgabe

	// Leap Constraint
	if (argc > 1 && strcmp(argv[1], "--bg") == 0)
		controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);



	Cube* cube = new Cube(vec3(2.0f, 2.0f, -10.0f), 1.0f);
	std::vector<std::string> attachShaders = { "/Test_Telepresence/phong.vert", "/Test_Telepresence/phong.frag" };
	RenderPass* cubePass = new RenderPass(
		cube,
		new ShaderProgram(attachShaders));
	
	Sphere* sphere = new Sphere(0.1f);
	RenderPass* spherePass = new RenderPass(
		sphere,
		new ShaderProgram(attachShaders));
	
	//std::vector<std::string> textureShaders = { "/Test_Telepresence/fullscreen.vert", "/Test_Telepresence/simpleTexture.frag" };
	//Quad* quad = new Quad();
	//RenderPass* texturePass = new RenderPass(
	//	quad,
	//	new ShaderProgram(textureShaders));

	PointCloud* pointCloud = new PointCloud();
	std::vector<std::string> attachMinimalShaders = { "/Test_Telepresence/minimal.vert", "/Test_Telepresence/minimal.frag" };
	RenderPass* pointCloudPass = new RenderPass(
		pointCloud,
		new ShaderProgram(attachMinimalShaders));






	mat4 modelMatrixJoints = mat4(1.0f);
	mat4 modelMatrixGoal = mat4(1.0f);
	vec3 lightPos = vec3(2.0f, 2.0f, 2.0f);
	
	cubePass
		->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);

	spherePass
		->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);

	pointCloudPass
		->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);

	colorData = new float[depthWidth * depthHeight * 3];
	positionData = new float[depthWidth * depthHeight * 3];

	int count = 0;
	render(window, [&](float deltaTime, glm::mat4 projection, glm::mat4 view) {
		//cout << count++ << " " << deltaTime << endl;

		glfwGetWindowSize(window, &width, &height);

		// clear screen
		//glClearColor(0.95, 0.95, 0.95, 1.0);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//view = glm::lookAt(glm::vec3(leftRight, upDown, nearFar), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		cubePass
			->update("lightPosition", lightPos)
			->update("diffuseColor", vec3(1.0, 0.0, 0.0))
			->update("projectionMatrix", projection)
			->update("viewMatrix", view);
		spherePass
			->update("lightPosition", lightPos)
			->update("diffuseColor", vec3(1.0, 0.0, 0.0))
			->update("projectionMatrix", projection)
			->update("viewMatrix", view);
		pointCloudPass
			->update("projectionMatrix", projection)
			->update("viewMatrix", view);

		//texturePass
		//	->update("resolution", getResolution(window));
		



		

		//tex = new float[depthWidth * depthHeight * 3 * sizeof(float)];

		kinectHandler.update(positionData, colorData);
		pointCloud->updatePointCloud(positionData, colorData);
		
		//delete[] colorData;
		//delete[] positionData;



		////bind the texture
		//glBindTexture(GL_TEXTURE_2D, texture);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, depthWidth, depthHeight, 0, GL_RGB, GL_FLOAT, colorData);

		////texture settings
		//glGenerateMipmap(GL_TEXTURE_2D);
		//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, true);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		//glBindTexture(GL_TEXTURE_2D, 0);



		vector<Bone> bones = leapHandler.getBoneList(controller);
		InteractionBox box = controller.frame().interactionBox();

		

		//draw Goal
		cubePass
			->update("modelMatrix", modelMatrixGoal)
			->run();

		//draw Bones
		if (bones.size() == 0)
			spherePass
				->update("modelMatrix", modelMatrixJoints)
				->run();
		else{
			for (int i = 0; i < bones.size(); i++)
			{
				modelMatrixJoints = mat4(1.0f);

				//compute rotation and translation 
				mat4 rotationMat = leapHandler.convertLeapMatToGlm(bones[i].basis());
				mat4 translateMat = translate(modelMatrixJoints, leapHandler.convertLeapVecToGlm(box.normalizePoint(bones[i].nextJoint(), false)) * 2.0f - 1.0f);
				mat4 finalMat = translateMat * rotationMat;
				
				//draw Bone 
				spherePass
					->update("modelMatrix", finalMat)
					->run();
			}

			//compute direction and translation for pointing bone
			modelMatrixJoints = mat4(1.0f);
			vec3 eins = leapHandler.convertLeapVecToGlm(box.normalizePoint(bones[7].nextJoint(), false)) * 2.0f - 1.0f;
			vec3 zwei = leapHandler.convertLeapVecToGlm(box.normalizePoint(bones[5].prevJoint(), false)) * 2.0f - 1.0f;
			vec3 dTest = eins - zwei; 
			normalize(dTest);

			

			vec3 directionTest1 = dTest;
			mat4 boneTestRot = leapHandler.convertLeapMatToGlm(bones[7].basis());
			mat4 boneTest = translate(modelMatrixJoints, leapHandler.convertLeapVecToGlm(box.normalizePoint(bones[7].nextJoint(), false)) * 2.0f - 1.0f);
			mat4 dirMatTest = boneTest; //* boneTestRot;

			//show pointing direction 
			spherePass
				->update("modelMatrix", translate(dirMatTest, directionTest1 * 1.0f))
				->run();
			spherePass
				->update("modelMatrix", translate(dirMatTest, directionTest1 * 2.6f))
				->run();
			spherePass
				->update("modelMatrix", translate(dirMatTest, directionTest1 * 4.2f))
				->run();
			spherePass
				->update("modelMatrix", translate(dirMatTest, directionTest1 * 5.8f))
				->run();
			spherePass
				->update("modelMatrix", translate(dirMatTest, directionTest1 * 7.4f))
				->run();
			spherePass
				->update("modelMatrix", translate(dirMatTest, directionTest1 * 9.0f))
				->run();
			spherePass
				->update("modelMatrix", translate(dirMatTest, directionTest1 * 10.6f))
				->run();
			spherePass
				->update("modelMatrix", translate(dirMatTest, directionTest1 * 12.2f))
				->run();






			//Hack fuer zusaetzliche Bobbels in linker und rechter Hand 
			modelMatrixJoints = mat4(1.0f);

			mat4 rotationMatFirstHand = leapHandler.convertLeapMatToGlm(bones[16].basis());
			mat4 translateMatFirstHand = translate(modelMatrixJoints, leapHandler.convertLeapVecToGlm(box.normalizePoint(bones[16].prevJoint(), false)) * 2.0f - 1.0f);
			mat4 finalMatFirstHand = translateMatFirstHand * rotationMatFirstHand;

			spherePass
				->update("modelMatrix", finalMatFirstHand)
				->run();

			if (bones.size() > 20){
				mat4 rotationMatSecondHand = leapHandler.convertLeapMatToGlm(bones[36].basis());
				mat4 translateMatSecondHand = translate(modelMatrixJoints, leapHandler.convertLeapVecToGlm(box.normalizePoint(bones[36].prevJoint(), false)) * 2.0f - 1.0f);
				mat4 finalMatSecondHand = translateMatSecondHand * rotationMatSecondHand;
			spherePass
				->update("modelMatrix", finalMatSecondHand)
				->run();
			}




			//check for intersection
			if (bones.size() != 0){
				bool a = leapHandler.checkForIntersection(cube->getVertices(), leapHandler.convertLeapVecToGlm(box.normalizePoint(bones[7].nextJoint(), false)) * 2.0f - 1.0f, directionTest1);
				//cout << "X: "<< directionTest1.x << ", Y: " << directionTest1.y << ", Z: " << directionTest1.z << endl;

				if (a == true){
					//cout << "Getroffen" << endl;
					cubePass
						->update("diffuseColor", vec3(0.0, 1.0, 0.0))
						->run();
				}
				else{
					//cout << "Nicht getroffen" << endl;
					cubePass
						->update("diffuseColor", vec3(1.0, 0.0, 0.0))
						->run();
				}
			}
		}



		//texturePass
		//	->clear(0.0, 0.0, 0.0, 1.0)
		//	->texture("tex", texture)
		//	//->texture("tex", TextureTools::loadTexture("D:/FP14/FP14_OpenGL_Framework/resources/bambus.jpg"))
		//	->run();

		//delete(tex);



		pointCloudPass
			//->clear(1.0, 1.0, 1.0, 1.0)
			->run();
		 
		//pointCloud->deleteBuffers(); // Brauchen wir nicht mehr?! Siehe PointCloud.cpp
	}); 
	
	return 0;
}
