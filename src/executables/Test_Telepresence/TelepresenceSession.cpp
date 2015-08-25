#include "TelepresenceSession.h"
#include "ShaderTools/OculusRenderer.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"
#include "ShaderTools/VertexArrayObjects/Sphere.h"
#include "LeapHandler.h"
#include "KinectHandler.h"
#include "AssimpLoader/AssimpLoader.h"
#include "TextPane.h"
#include "PointCloud.h"


//transform due to head mounted Leap Motion
const static glm::mat4 oculusToLeap(
	-1.0f, 0.0, 0.0, 0.0,
	0.0, 0.0, -1.0f, 0.0,
	0.0, -1.0f, 0.0, -0.08f,
	0.0, 0.0, 0.0, 1.0);

//transform from millimeter to meter
const static glm::mat4 normalizeMat(
	0.001f, 0.0, 0.0, 0.0,
	0.0, 0.001f, 0.0, 0.0,
	0.0, 0.0, 0.001f, 0.0,
	0.0, 0.0, 0.0, 1.0);

TelepresenceSession::TelepresenceSession()
{

}


TelepresenceSession::~TelepresenceSession()
{

}

void TelepresenceSession::init()
{
	generateOculusWindow();
	initOpenGL();
	m_assimpLoader = new AssimpLoader();
	m_kinectHandler = new KinectHandler();
	m_leapHandler = new LeapHandler();
	m_kinectHandler->initializeDefaultSensor();
	m_pointCloud = new PointCloud( m_kinectHandler);
	m_assimpLoader->loadFile(RESOURCES_PATH "/obj/room.obj")
		->printLog();
	initShaderPrograms();
	initRenderPasses();
}

void TelepresenceSession::run()
{
	render(m_window, [&]( double delta, glm::mat4 projection, glm::mat4 view)
	{
		renderLoop(delta, projection, view);
	});
}

void TelepresenceSession::renderLoop(double deltaTime, glm::mat4 projection, glm::mat4 view)
{
	updateProjectionMatrices(projection);
	updateViewMatrices(view);

	glm::vec3 cameraPosition = extractCameraPosition(view);

	renderBillboards(cameraPosition);
	renderRoom();
	renderLeap();
	renderPointCloud();
	renderTestCube();
}


void TelepresenceSession::generateOculusWindow()
{
	m_window = generateWindow();
}

void TelepresenceSession::initOpenGL()
{
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void TelepresenceSession::initShaderPrograms()
{
	m_phongShaders = new ShaderProgram({ "/Test_Telepresence/phong.vert", "/Test_Telepresence/phong.frag" });
	m_minimalShaders = new ShaderProgram({ "/Test_Telepresence/minimal.vert", "/Test_Telepresence/minimal.frag" });
	m_minimalMatShaders = new ShaderProgram({ "/Test_Telepresence/minimalmat.vert", "/Test_Telepresence/minimalmat.frag" });
	m_textureShaders = new ShaderProgram({ "/Test_Telepresence/texture.vert", "/Test_Telepresence/texture.frag" });
}

void TelepresenceSession::initRenderPasses()
{
	Cube* testCube = new Cube(glm::vec3(1.0f, 0.0f, -4.0f), .5f);
	Cube* directionCube = new Cube(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
	Sphere* sphere = new Sphere(10.0f);
	m_textPane = new TextPane(glm::vec3(-2.0f, 0.0f, -3.0f), 2.0f, 1.0f, "Herzlich Willkommen");

	m_cubePass = new RenderPass(testCube, m_phongShaders);
	m_billboardPass = new RenderPass(m_textPane, m_textureShaders);
	m_handPass = new RenderPass( sphere, m_phongShaders);
	m_roomPass = new RenderPass( m_assimpLoader->getMeshList()->at(0), m_minimalMatShaders);
	m_pointCloudPass = new RenderPass(m_pointCloud, m_minimalShaders);
	m_directionPass = new RenderPass( directionCube, m_phongShaders);

	glm::vec3 lightPos = glm::vec3(2.0f, 10.0f, 2.0f);

	m_cubePass
		->update("lightPosition", lightPos)
		->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
	m_billboardPass
		->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
	m_handPass
		->update("lightPosition", lightPos)
		->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
	m_roomPass
		->update("lightPosition", lightPos)
		->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
	m_pointCloudPass
		->update("lightPosition", lightPos)
		->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
	m_directionPass
		->update("lightPosition", lightPos)
		->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
}

void TelepresenceSession::deleteShaderPrograms()
{
	delete m_roomPass;
	delete m_pointCloudPass;
	delete m_directionPass;
	delete m_handPass;
	delete m_cubePass;
	delete m_billboardPass;
}

void TelepresenceSession::updateProjectionMatrices(glm::mat4 projection)
{
	m_cubePass
		->update("projectionMatrix", projection);
	m_billboardPass
		->update("projectionMatrix", projection);
	m_handPass
		->update("projectionMatrix", projection);
	m_roomPass
		->update("projectionMatrix", projection);
	m_pointCloudPass
		->update("projectionMatrix", projection);
	m_directionPass
		->update("projectionMatrix", projection);
}

void TelepresenceSession::updateViewMatrices(glm::mat4 view)
{
	m_cubePass
		->update("viewMatrix", view);
	m_billboardPass
		->update("viewMatrix", view);
	m_handPass
		->update("viewMatrix", view);
	m_roomPass
		->update("viewMatrix", view);
	m_pointCloudPass
		->update("viewMatrix", view);
	m_directionPass
		->update("viewMatrix", view);
}

glm::vec3 TelepresenceSession::extractCameraPosition(glm::mat4 viewMatrix)
{
	glm::mat3 rotMat(viewMatrix);
	glm::vec3 direction(viewMatrix[3]);
	return -direction * rotMat;
}

void TelepresenceSession::renderRoom()
{
	//m_roomPass->run();
	for (unsigned int m = 0; m < m_assimpLoader->getMeshList()->size(); ++m)
	{
		glm::mat4 model = glm::rotate(m_assimpLoader->getModelMatrix(m), 0.0f, glm::vec3(0, 1, 0));
		m_minimalMatShaders->update("model", model);
		m_minimalMatShaders->update("materialColor", m_assimpLoader->getMaterialColor(m_assimpLoader->getMeshList()->at(m)->getMaterialIndex()));
		m_roomPass->getFrameBufferObject()->bind();
		m_roomPass->getShaderProgram()->use();
		m_assimpLoader->getMeshList()->at(m)->draw();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void TelepresenceSession::renderPointCloud()
{
	m_pointCloud->updatePointCloud();
	m_pointCloudPass->run();
}

void TelepresenceSession::renderTestCube()
{
	m_cubePass
		->update("diffuseColor", glm::vec3(.0, 1.0, .0))
		->update("modelMatrix", glm::mat4(1.0f))
		->run();
}

void TelepresenceSession::renderBillboards(glm::vec3 cameraPosition)
{
	glm::mat4 met = m_textPane->getBillboardModelMatrix(cameraPosition);
	m_billboardPass
		->update("modelMatrix", met)
		->texture("tex", m_textPane->getTextureHandle())
		->run();

}

void TelepresenceSession::renderLeap()
{
	m_leapHandler->updateLeap();
	m_handPass
		->update("diffuseColor", glm::vec3(0.2f, 0.2f, 0.2f));
	leapChaosFunc();
}

void TelepresenceSession::leapChaosFunc()
{
	vector<Bone> bones = m_leapHandler->getBoneList();
	//InteractionBox box = controller.frame().interactionBox();

	//transform world coordinates into Oculus coordinates (for attached Leap Motion)
	//ovrPosef headPose = ovrHmd_GetTrackingState(g_Hmd, ovr_GetTimeInSeconds()).HeadPose.ThePose;
	ovrPosef headPose = ovrHmd_GetTrackingState(g_Hmd, 0.0f).HeadPose.ThePose;
	glm::mat4 M_trans = toGlm(OVR::Matrix4f::Translation(headPose.Position));
	glm::mat4 M_rot = toGlm(OVR::Matrix4f(headPose.Orientation));

	glm::mat4 leapToOculusPipeline = M_trans * M_rot * oculusToLeap * normalizeMat;

	//draw Bones
	if (bones.size() != 0){
		for (int i = 0; i < bones.size(); i++)
		{
			//compute rotation and translation of Leap Motion Bones
			glm::mat4 rotationMat = m_leapHandler->convertLeapMatToGlm(bones[i].basis());
			glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), m_leapHandler->convertLeapVecToGlm(bones[i].nextJoint()));
			glm::mat4 leapWorldCoordinates = translateMat * rotationMat;

			//Pipeline for transforming Leap Motion bones
			glm::mat4 finalMat = leapToOculusPipeline * leapWorldCoordinates;

			//draw Bone 
			m_handPass
				->update("modelMatrix", finalMat)
				->run();
		}

		//additional hand bone 1st hand
		glm::mat4 rotationMatFirstHand = m_leapHandler->convertLeapMatToGlm(bones[16].basis());
		glm::mat4 translateMatFirstHand = glm::translate(glm::mat4(1.0f), m_leapHandler->convertLeapVecToGlm(bones[16].prevJoint()));
		glm::mat4 finalMatFirstHand = translateMatFirstHand * rotationMatFirstHand;

		glm::mat4 finalMatHack = leapToOculusPipeline * finalMatFirstHand;

		m_handPass
			->update("modelMatrix", finalMatHack)
			->run();

		//additional hand bone 2nd hand
		if (bones.size() > 20){
			glm::mat4 rotationMatSecondHand = m_leapHandler->convertLeapMatToGlm(bones[36].basis());
			glm::mat4 translateMatSecondHand = glm::translate(glm::mat4(1.0f), m_leapHandler->convertLeapVecToGlm(bones[36].prevJoint()));
			glm::mat4 finalMatSecondHand = translateMatSecondHand * rotationMatSecondHand;

			glm::mat4 finalMatHack2 = leapToOculusPipeline * finalMatSecondHand;

			m_handPass
				->update("modelMatrix", finalMatHack2)
				->run();
		}

		// TODO: DIRECTION FUER DEN INTERSECTION TEST MUSS IRGENDWIE AUCH DURCH DIE "PIPELINE" GEJAGT WERDEN?!?!?!

		if (m_leapHandler->leftHandPinched){
			//compute rotation and translation of Leap Motion Bones
			glm::mat4 palmRotationMat = m_leapHandler->convertLeapMatToGlm(m_leapHandler->rightHand.basis());
			glm::mat4 palmTranslateMat = glm::translate(glm::mat4(1.0f), m_leapHandler->convertLeapVecToGlm(m_leapHandler->rightHand.palmPosition()));
			glm::mat4 palmLeapWorldCoordinates = palmTranslateMat * palmRotationMat;

			//Pipeline for transforming Leap Motion bones
			glm::mat4 palmFinalMat = leapToOculusPipeline * palmLeapWorldCoordinates;

			//draw Bone 
			m_directionPass
				->update("modelMatrix", glm::translate(glm::scale(palmFinalMat, glm::vec3(1.0, 100.0, 1.0)), glm::vec3(0.0, -3.0, 0.0)))

				->run();
		}

		//compute direction and translation for pointing bone
		glm::vec4 eins = glm::vec4(m_leapHandler->convertLeapVecToGlm(bones[7].nextJoint()), 1.0f);
		glm::vec4 zwei = glm::vec4(m_leapHandler->convertLeapVecToGlm(bones[5].prevJoint()), 1.0f);

		glm::vec4 directionTest = glm::normalize(eins - zwei);
		glm::vec3 directionTest2(directionTest.x, directionTest.y, directionTest.z);

		glm::mat4 boneTestRot = m_leapHandler->convertLeapMatToGlm(bones[7].basis());
		glm::mat4 boneTest = glm::translate(glm::mat4(1.0f), m_leapHandler->convertLeapVecToGlm(bones[7].nextJoint()));
		glm::mat4 finalDirMatTest = boneTest; // * boneTestRot;

		glm::mat4 boneOrigin = leapToOculusPipeline* finalDirMatTest;

		glm::vec4 boneOrigin2 = leapToOculusPipeline * glm::vec4(m_leapHandler->convertLeapVecToGlm(bones[7].nextJoint()), 1.0);

		eins = M_trans * M_rot * oculusToLeap * normalizeMat * eins;
		zwei = M_trans * M_rot * oculusToLeap * normalizeMat * zwei;
		glm::vec4 drei = eins - zwei;

		glm::vec3 vier = glm::normalize(glm::vec3(drei.x, drei.y, drei.z));

		//check for intersection
		//if (bones.size() != 0){
		//	bool a = m_leapHandler->checkForIntersection(cube->getVertices(), glm::vec3(boneOrigin2.x, boneOrigin2.y, boneOrigin2.z), vier);
		//	//cout << "X: "<< directionTest1.x << ", Y: " << directionTest1.y << ", Z: " << directionTest1.z << endl;

		//	if (a == true){
		//		//cout << "Getroffen" << endl;
		//		cubePass
		//			->update("diffuseColor", glm::vec3(0.0, 1.0, 0.0))
		//			->run();
		//	}
		//	else{
		//		//cout << "Nicht getroffen" << endl;
		//		cubePass
		//			->update("diffuseColor", glm::vec3(1.0, 0.0, 0.0))
		//			->run();
		//	}
		//}
	}
}