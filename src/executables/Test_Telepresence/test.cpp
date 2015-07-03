#include "ShaderTools/OculusRenderer.h"
//#include "ShaderTools/Renderer.h"
#include "ShaderTools/RenderPass.h"
#include "Compression/TextureTools.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"
#include "ShaderTools/VertexArrayObjects/Sphere.h"
#include "PointCloud.h"
#include "TextButton.h"
#include "TextPane.h"
#include "LeapMotionHandler.h"
#include "KinectHandler.h"
#include <typeinfo>

using namespace std;
using namespace glm;

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



	//transform due to head mounted Leap Motion
	float tz = -0.08f;
	glm::mat4 oculusToLeap(-1.0f, 0.0, 0.0, 0.0,
							0.0, 0.0, -1.0f, 0.0,
							0.0, -1.0f, 0.0, tz,
							0.0, 0.0, 0.0, 1.0);

	//transform from millimeter to meter
	glm::mat4 normalizeMat(	0.001f, 0.0, 0.0, 0.0,
							0.0, 0.001f, 0.0, 0.0,
							0.0, 0.0, 0.001f, 0.0,
							0.0, 0.0, 0.0, 1.0);


	
	Cube* cube = new Cube(vec3(1.0f, 1.0f, -7.0f), 1.0f);
	std::vector<std::string> attachShaders = { "/Test_Telepresence/phong.vert", "/Test_Telepresence/phong.frag" };
	RenderPass* cubePass = new RenderPass(
		cube,
		new ShaderProgram(attachShaders));

	// Textured Button Test Object
	std::vector<std::string> attachTextureShaders = { "/Test_Telepresence/texture.vert", "/Test_Telepresence/texture.frag" };
	TextPane* texButton = new TextPane(vec3(2.0f, 2.0f, -4.0f), 2.0f, 1.0f, "SAHEN");
	RenderPass* texButtonPass = new RenderPass(
		texButton,
		new ShaderProgram(attachTextureShaders)
		);
	
	Sphere* sphere = new Sphere(10.0f);
	RenderPass* spherePass = new RenderPass(
		sphere,
		new ShaderProgram(attachShaders));

	PointCloud* pointCloud = new PointCloud();
	std::vector<std::string> attachMinimalShaders = { "/Test_Telepresence/minimal.vert", "/Test_Telepresence/minimal.frag" };
	RenderPass* pointCloudPass = new RenderPass(
		pointCloud,
		new ShaderProgram(attachMinimalShaders));



	// initialize texture button test object
	
	texButtonPass
		->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);

	cubePass
		->update("diffuseColor", vec3(0.0, 1.0, 0.0))
		->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);

	spherePass
		->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);

	pointCloudPass
		->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);

	colorData = new float[depthWidth * depthHeight * 3];
	positionData = new float[depthWidth * depthHeight * 3];

vec3 lightPos = vec3(2.0f, 2.0f, 2.0f);
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	int count = 0;
	render(window, [&](float deltaTime, glm::mat4 projection, glm::mat4 view) {
		//cout << count++ << " " << deltaTime << endl;
		glfwGetWindowSize(window, &width, &height);

		cubePass
			->update("lightPosition", lightPos)
			->update("projectionMatrix", projection)
			->update("viewMatrix", view);
		texButtonPass
			//->update("lightPosition", lightPos)
			//->clear(0, 0, 0, 0)
			->texture("tex", texButton->getTextureHandle())
			->update("projectionMatrix", projection)
			->update("viewMatrix", view);
		spherePass
			->update("lightPosition", lightPos)
			->update("diffuseColor", vec3(0.2f, 0.2f, 0.2f))
			->update("projectionMatrix", projection)
			->update("viewMatrix", view);
		pointCloudPass
			->update("projectionMatrix", projection)
			->update("viewMatrix", view);
		kinectHandler.update(positionData, colorData);
		pointCloud->updatePointCloud(positionData, colorData);
		vector<Bone> bones = leapHandler.getBoneList(controller);
		//InteractionBox box = controller.frame().interactionBox();

		//cubePass
		//	->update("modelMatrix", modelMatrixGoal)
		//	->run();
		texButtonPass
			->update("modelMatrix", mat4(1.0f))
			->run();



		//transform world coordinates into Oculus coordinates (for attached Leap Motion)
		//ovrPosef headPose = ovrHmd_GetTrackingState(g_Hmd, ovr_GetTimeInSeconds()).HeadPose.ThePose;
		ovrPosef headPose = ovrHmd_GetTrackingState(g_Hmd, 0.0f).HeadPose.ThePose;
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






			// TODO: DIRECTION FUER DEN INTERSECTION TEST MUSS IRGENDWIE AUCH DURCH DIE "PIPELINE" GEJAGT WERDEN?!?!?!

			//compute direction and translation for pointing bone
			vec4 eins = vec4(leapHandler.convertLeapVecToGlm(bones[7].nextJoint()), 1.0f);
			vec4 zwei = vec4(leapHandler.convertLeapVecToGlm(bones[5].prevJoint()), 1.0f);
			
			vec4 directionTest = normalize(eins - zwei);
			vec3 directionTest2(directionTest.x, directionTest.y, directionTest.z);
						
			mat4 boneTestRot = leapHandler.convertLeapMatToGlm(bones[7].basis());
			mat4 boneTest = translate(mat4(1.0f), leapHandler.convertLeapVecToGlm(bones[7].nextJoint()));
			mat4 finalDirMatTest = boneTest; // * boneTestRot;

			mat4 boneOrigin = M_trans * M_rot * oculusToLeap * normalizeMat * finalDirMatTest;

			vec4 boneOrigin2 = M_trans * M_rot * oculusToLeap * normalizeMat * vec4(leapHandler.convertLeapVecToGlm(bones[7].nextJoint()), 1.0);

			//show pointing direction 
			spherePass
				->update("modelMatrix", translate(boneOrigin, directionTest2 * 5.0f))
				->run();
			spherePass
				->update("modelMatrix", translate(boneOrigin, directionTest2 * 15.0f))
				->run();
			spherePass
				->update("modelMatrix", translate(boneOrigin, directionTest2 * 25.0f))
				->run();
			spherePass
				->update("modelMatrix", translate(boneOrigin, directionTest2 * 35.0f))
				->run();
			spherePass
				->update("modelMatrix", translate(boneOrigin, directionTest2 * 45.0f))
				->run();
			spherePass
				->update("modelMatrix", translate(boneOrigin, directionTest2 * 55.0f))
				->run();
			spherePass
				->update("modelMatrix", translate(boneOrigin, directionTest2 * 65.0f))
				->run();
			spherePass
				->update("modelMatrix", translate(boneOrigin, directionTest2 * 75.0f))
				->run();
			



			eins = M_trans * M_rot * oculusToLeap * normalizeMat * eins;
			zwei = M_trans * M_rot * oculusToLeap * normalizeMat * zwei;
			vec4 drei = eins - zwei;

			vec3 vier = normalize(vec3(drei.x, drei.y, drei.z));

			//check for intersection
			if (bones.size() != 0){
				bool a = leapHandler.checkForIntersection(cube->getVertices(), vec3(boneOrigin2.x, boneOrigin2.y, boneOrigin2.z), vier);
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

		pointCloudPass
			->run();
		 
		//pointCloud->deleteBuffers(); // Brauchen wir nicht mehr?! Siehe PointCloud.cpp
	}); 
	
	return 0;
}
