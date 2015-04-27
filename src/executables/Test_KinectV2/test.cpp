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

	// Leap Constraint
	if (argc > 1 && strcmp(argv[1], "--bg") == 0)
		controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);



	// Initialize Kinect
	KinectHandler kinectHandler;
	kinectHandler.initializeDefaultSensor();

	GLfloat *colorData;
	GLfloat *positionData;
	//tex = (float*)malloc(depthWidth * depthHeight * 3 * sizeof(float)); // fuer Texturausgabe



	//transform due to head mounted Leap Motion
	float tz = -0.08f;
	glm::mat4 oculusToLeap(-1.0f, 0.0, 0.0, 0.0,
		0.0, 0.0, -1.0f, 0.0,
		0.0, -1.0f, 0.0, tz,
		0.0, 0.0, 0.0, 1.0);

	//transform from millimeter to meter
	glm::mat4 normalizeMat(0.001f, 0.0, 0.0, 0.0,
		0.0, 0.001f, 0.0, 0.0,
		0.0, 0.0, 0.001f, 0.0,
		0.0, 0.0, 0.0, 1.0);


	
	Cube* cube = new Cube(vec3(2.0f, 2.0f, -10.0f), 1.0f);
	std::vector<std::string> attachShaders = { "/Test_Telepresence/phong.vert", "/Test_Telepresence/phong.frag" };
	RenderPass* cubePass = new RenderPass(
		cube,
		new ShaderProgram(attachShaders));
	
	Sphere* sphere = new Sphere(10.0f);
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


	
	cubePass
		->update("diffuseColor", vec3(1.0, 0.0, 0.0))
		->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);

	spherePass
		->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);

	pointCloudPass
		->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);

	colorData = new float[depthWidth * depthHeight * 3];
	positionData = new float[depthWidth * depthHeight * 3];

	vec3 lightPos = vec3(2.0f, 2.0f, 2.0f);

	int count = 0;
	render(window, [&](float deltaTime, glm::mat4 projection, glm::mat4 view) {
		//cout << count++ << " " << deltaTime << endl;

		glfwGetWindowSize(window, &width, &height);

		// clear screen
		//glClearColor(0.95, 0.95, 0.95, 1.0);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cubePass
			->update("lightPosition", lightPos)
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
		//InteractionBox box = controller.frame().interactionBox();

		//draw Goal
		cubePass
			->update("modelMatrix", mat4(1.0f))
			->run();



		//transform world coordinates into Oculus coordinates (for attached Leap Motion)
		ovrPosef headPose = ovrHmd_GetTrackingState(g_Hmd, ovr_GetTimeInSeconds()).HeadPose.ThePose;
		glm::mat4 M_trans = toGlm(OVR::Matrix4f::Translation(headPose.Position));
		glm::mat4 M_rot = toGlm(OVR::Matrix4f(headPose.Orientation));

		//draw Bones
		if (bones.size() != 0){
			for (int i = 0; i < bones.size(); i++)
			{
				//compute rotation and translation of Leap Motion Bones
				mat4 rotationMat = leapHandler.convertLeapMatToGlm(bones[i].basis());
				mat4 translateMat = translate(mat4(1.0f), leapHandler.convertLeapVecToGlm(bones[i].nextJoint()));
				mat4 leapWorldCoordinates = translateMat * rotationMat;

				//Pipeline for transforming Leap Motion bones
				mat4 finalMat = M_trans * M_rot * oculusToLeap * normalizeMat * leapWorldCoordinates;
				
				//draw Bone 
				spherePass
					->update("modelMatrix", finalMat)
					->run();
			}

			//Hack fuer zusaetzlichen Bobbel in erster Hand 
			mat4 rotationMatFirstHand = leapHandler.convertLeapMatToGlm(bones[16].basis());
			mat4 translateMatFirstHand = translate(mat4(1.0f), leapHandler.convertLeapVecToGlm(bones[16].prevJoint()));
			mat4 finalMatFirstHand = translateMatFirstHand * rotationMatFirstHand;

			mat4 finalMatHack = M_trans * M_rot * oculusToLeap * normalizeMat * finalMatFirstHand;

			spherePass
				->update("modelMatrix", finalMatHack)
				->run();

			
			//Hack fuer zusaetzlichen Bobbel in zweiter Hand 
			if (bones.size() > 20){
				mat4 rotationMatSecondHand = leapHandler.convertLeapMatToGlm(bones[36].basis());
				mat4 translateMatSecondHand = translate(mat4(1.0f), leapHandler.convertLeapVecToGlm(bones[36].prevJoint()));
				mat4 finalMatSecondHand = translateMatSecondHand * rotationMatSecondHand;

				mat4 finalMatHack2 = M_trans * M_rot * oculusToLeap * normalizeMat * finalMatSecondHand;

				spherePass
					->update("modelMatrix", finalMatHack2)
					->run();
			}








			////compute direction and translation for pointing bone
			//vec3 eins = leapHandler.convertLeapVecToGlm(bones[7].nextJoint());
			//vec3 zwei = leapHandler.convertLeapVecToGlm(bones[5].prevJoint());
			//vec3 directionTest = normalize(eins - zwei);
			//
			//
			//
			//mat4 boneTestRot = leapHandler.convertLeapMatToGlm(bones[7].basis());
			//mat4 boneTest = translate(mat4(1.0f), leapHandler.convertLeapVecToGlm(bones[7].nextJoint()));
			//mat4 finalDirMatTest = boneTest; // *boneTestRot;

			//mat4 boneOrigin = M_trans * M_rot * oculusToLeap * normalizeMat * finalDirMatTest;

			////show pointing direction 
			//spherePass
			//	->update("modelMatrix", translate(boneOrigin, directionTest * 5.0f))
			//	->run();
			//spherePass
			//	->update("modelMatrix", translate(boneOrigin, directionTest * 15.0f))
			//	->run();
			//spherePass
			//	->update("modelMatrix", translate(boneOrigin, directionTest * 25.0f))
			//	->run();
			//spherePass
			//	->update("modelMatrix", translate(boneOrigin, directionTest * 35.0f))
			//	->run();
			//spherePass
			//	->update("modelMatrix", translate(boneOrigin, directionTest * 45.0f))
			//	->run();
			//spherePass
			//	->update("modelMatrix", translate(boneOrigin, directionTest * 55.0f))
			//	->run();
			//spherePass
			//	->update("modelMatrix", translate(boneOrigin, directionTest * 65.0f))
			//	->run();
			//spherePass
			//	->update("modelMatrix", translate(boneOrigin, directionTest * 75.0f))
			//	->run();











			////check for intersection
			//if (bones.size() != 0){
			//	bool a = leapHandler.checkForIntersection(cube->getVertices(), leapHandler.convertLeapVecToGlm(bones[7].nextJoint()), directionTest);
			//	//cout << "X: "<< directionTest1.x << ", Y: " << directionTest1.y << ", Z: " << directionTest1.z << endl;

			//	if (a == true){
			//		//cout << "Getroffen" << endl;
			//		cubePass
			//			->update("diffuseColor", vec3(0.0, 1.0, 0.0))
			//			->run();
			//	}
			//	else{
			//		//cout << "Nicht getroffen" << endl;
			//		cubePass
			//			->update("diffuseColor", vec3(1.0, 0.0, 0.0))
			//			->run();
			//	}
			//}
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
