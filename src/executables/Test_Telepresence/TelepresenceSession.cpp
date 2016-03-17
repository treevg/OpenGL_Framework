#include "TelepresenceSession.h"
#include "ShaderTools/OculusRenderer.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"
#include "ShaderTools/VertexArrayObjects/Sphere.h"
#include "LeapHandler.h"
#include "KinectHandler.h"
#include "AssimpLoader/AssimpLoader.h"
#include "TextPane.h"
#include "PointCloud.h"
#include "CameraObjectRelations.h"
#include "controls.hpp"
#include "ShaderTools/VertexArrayObjects/Quad.h"

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
	std::cout << "Init Session" << std::endl;
	generateOculusWindow();
	initOpenGL();
	initMouseAndKeyboardMovement();
	initFramesCounter();
	m_assimpLoader = new AssimpLoader();
	m_kinectHandler = new KinectHandler();
	m_leapHandler = new LeapHandler();
	m_kinectHandler->initializeDefaultSensor();
	m_pointCloud = new PointCloud(m_kinectHandler);
	m_assimpLoader->loadFile(RESOURCES_PATH "/obj/room2_tris.obj");
	//m_kinectHandler->retrieveCameraIntrinsics();
	m_assimpLoader->loadFile(RESOURCES_PATH "/obj/room.obj")
		->printLog();
	m_hhfVao == NULL;

	generateHoleFillingAssets();

	initShaderPrograms();
	initRenderPasses();
	
}

void TelepresenceSession::run()
{

	render(m_window, [&](double delta, glm::mat4 projection, glm::mat4 view)
		{
			//m_kinectHandler->retrieveCameraIntrinsics();
			renderLoop(delta, projection, view);

		}, 
		[&]()
		{
			performHHF();
		}
	);
}

void TelepresenceSession::renderLoop(double deltaTime, glm::mat4 projection, glm::mat4 view)
{
		measureSpeedOfApplication();
		if (m_toggle_mouseAsCamera) {
			computeMatricesFromInputs(m_window);
			//projection = getProjectionMatrix();
			view = getViewMatrix();
		}
		updateProjectionMatrices(projection);
		updateViewMatrices(view);

		glm::vec3 cameraPosition = extractCameraPosition(view);

		if (m_toggle_userInfo) {
			//renderBillboards(cameraPosition);
		}
		//renderPanels();
		renderRoom(cameraPosition);
		renderTestCube();

		if (m_toggle_leapMotion) {
				renderLeap(cameraPosition);
		}

		if (m_toggle_pointcloud) {
			renderPointCloud();
		}

		if (m_toggle_hud) {
			//glDepthFunc(GL_ALWAYS);
			///renderHud(cameraPosition);
			//glDepthFunc(GL_LESS);
		}
		//renderResult();
}

void TelepresenceSession::performHHF(){
	renderHHF();
	renderResult();
	glBindFramebuffer(GL_FRAMEBUFFER, m_hhfMipmapFBOHandles[0]);
	glClear(GL_COLOR_BUFFER_BIT);
}

void TelepresenceSession::generateOculusWindow()
{
	m_window = generateWindow();
}

void TelepresenceSession::keycallback(GLFWwindow* p_Window, int p_Key, int scancode, int p_Action, int mods)
{
	if (p_Action == GLFW_PRESS || p_Action == GLFW_REPEAT)
	{
		switch (p_Key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(p_Window, GL_TRUE);
			break;
		case GLFW_KEY_R:
			RecenterPose();
			break;
		case GLFW_KEY_H:
			// ENABLE/DISABLE HUD
			m_toggle_hud = !m_toggle_hud;
			break;
		case GLFW_KEY_U:
			// ENABLE/DISABLE USER_POINTCLOUD
			m_toggle_pointcloud = !m_toggle_pointcloud;
			break;
		case GLFW_KEY_I:
			// ENABLE/DISABLE USER_INFO
			m_toggle_userInfo = !m_toggle_userInfo;
			break;
		case GLFW_KEY_C:
			// ENABLE/DISABLE PRESENTATION BILLBOARD CANVAS
			m_toggle_presentationCanvas = !m_toggle_presentationCanvas;
			break;
		case GLFW_KEY_L:
			// ENABLE/DISABLE LEAP MOTION
			m_toggle_leapMotion = !m_toggle_leapMotion;
			break;
		case GLFW_KEY_K:
			// ENABLE/DISABLE MOUSE MOVEMENT (CAMERA AS MOUSE)
			m_toggle_mouseAsCamera = !m_toggle_mouseAsCamera;
			break;
		case GLFW_KEY_M:
			// ENABLE/DISABLE MOUSE CURSOR
			m_toggle_mouseCursor = !m_toggle_mouseCursor;
			break;
		}
	}
}

//void keyBoardInputs2(GLFWwindow* p_Window, int p_Key, int scancode, int p_Action, int mods)
//{
//	keyBoardInputs(p_Window, p_Key, scancode, p_Action, mods);
//}

void TelepresenceSession::initMouseAndKeyboardMovement()
{
	// Hide the mouse and enable unlimited movement
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Keyboard Callback Method
	glfwSetKeyCallback(m_window, keycallback_dispatch);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	int width, height;

	glfwGetWindowSize(m_window, &width, &height);

	/*int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);*/

	glfwSetCursorPos(m_window, width / 2, height / 2);
}

void TelepresenceSession::initOpenGL()
{
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void TelepresenceSession::initFramesCounter()
{
	lastTime = glfwGetTime();
	nbFrames = 0;
}

void TelepresenceSession::measureSpeedOfApplication()
{
	// Measure speed
	double currentTime = glfwGetTime();
	nbFrames++;

	if (currentTime - lastTime >= 1.0){ // If last prinf() was more than 1 sec ago
		// printf and reset timer
		printf("%f ms/frame\n", 1000.0 / double(nbFrames));
		nbFrames = 0;
		lastTime += 1.0;
	}
}

void TelepresenceSession::initShaderPrograms()
{
	// Phong Shaders
	m_cubeShaders = new ShaderProgram({ "/Test_Telepresence/phong.vert", "/Test_Telepresence/phong.frag" });
	m_directionShaders = new ShaderProgram({ "/Test_Telepresence/phong.vert", "/Test_Telepresence/phong.frag" });
	m_handShaders = new ShaderProgram({ "/Test_Telepresence/phong.vert", "/Test_Telepresence/phong.frag" });
	m_roomShaders = new ShaderProgram({ "/Test_Telepresence/phong.vert", "/Test_Telepresence/phong.frag" });

	// Texture Shaders
	m_billboardShaders = new ShaderProgram({ "/Test_Telepresence/texture.vert", "/Test_Telepresence/texture.frag" });
	m_hudShaders = new ShaderProgram({ "/Test_Telepresence/texture.vert", "/Test_Telepresence/texture.frag" });
	m_panelShaders = new ShaderProgram({ "/Test_Telepresence/texture.vert", "/Test_Telepresence/texture.frag" });

	// Point Cloud Shaders 
	m_pointCloudShaders = new ShaderProgram({ "/Test_Telepresence/pointcloudNoGeo.vert", "/Test_Telepresence/pointcloudNoGeo.frag" });

	// HoleFilling Shaders
	m_hhfReduceShaders = new ShaderProgram({ "/Test_Telepresence/hhf.vert", "/Test_Telepresence/reduce.frag" });
	m_hhfFillShaders = new ShaderProgram({ "/Test_Telepresence/hhf.vert", "/Test_Telepresence/fill.frag" });
	m_resultShaders = new ShaderProgram({ "/Test_Telepresence/result.vert", "/Test_Telepresence/result.frag" });
}

void TelepresenceSession::initRenderPasses()
{
	Cube* testCube = new Cube(glm::vec3(0.0f, 0.0f, 0.0f), .1f);
	Cube* directionCube = new Cube(glm::vec3(0.0f, 0.0f, 0.0f), 2.0f);
	Sphere* sphere = new Sphere(10.0f);
	Quad* quad = new Quad();
	m_textPane = new TextPane(0.8f, .4f, "Matthias");
	m_hud = new TextPane(0.8f, .4f, "", 20);

	m_textPanel = new TextPane(2.0f, 1.0f, "Wand");

	m_cubePass = new RenderPass(testCube, m_cubeShaders);
	m_billboardPass = new RenderPass(m_textPane, m_billboardShaders);
	m_hudPass = new RenderPass(m_hud, m_hudShaders);
	m_panelPass = new RenderPass(m_textPanel, m_panelShaders);
	m_handPass = new RenderPass(sphere, m_handShaders);
	m_roomPass = new RenderPass(m_assimpLoader->getMeshList()->at(0), m_roomShaders);
	m_pointCloudPass = new RenderPass(m_pointCloud, m_pointCloudShaders);
	m_directionPass = new RenderPass(directionCube, m_directionShaders);
	m_hhfReducePass = new RenderPass(m_hhfVao, m_hhfReduceShaders);
	m_hhfFillPass = new RenderPass(m_hhfVao, m_hhfFillShaders);
	m_resultPass = new RenderPass(m_hhfVao, m_resultShaders);

	glm::vec3 lightPos = glm::vec3(0.0f, 4.0f, 2.0f);

	m_cubePass
		->update("lightPosition", lightPos)
		->getFrameBufferObject()->setFrameBufferObjectHandle(m_hhfMipmapFBOHandles[0]);
	m_billboardPass
		->getFrameBufferObject()->setFrameBufferObjectHandle(m_hhfMipmapFBOHandles[0]);
	m_hudPass
		->getFrameBufferObject()->setFrameBufferObjectHandle(m_hhfMipmapFBOHandles[0]);
	m_panelPass
		->getFrameBufferObject()->setFrameBufferObjectHandle(m_hhfMipmapFBOHandles[0]);
	m_handPass
		->update("lightPosition", lightPos)
		->getFrameBufferObject()->setFrameBufferObjectHandle(m_hhfMipmapFBOHandles[0]);
	m_roomPass
		->update("lightPosition", lightPos)
		->getFrameBufferObject()->setFrameBufferObjectHandle(m_hhfMipmapFBOHandles[0]);
	m_pointCloudPass
		->getFrameBufferObject()->setFrameBufferObjectHandle(m_hhfMipmapFBOHandles[0]);
	m_directionPass
		->update("lightPosition", lightPos)
		->getFrameBufferObject()->setFrameBufferObjectHandle(m_hhfMipmapFBOHandles[0]);
	m_hhfReducePass
		->update("m_hhfResolution", m_hhfResolution)
		->getFrameBufferObject()->setFrameBufferObjectHandle(m_hhfMipmapFBOHandles[0]);
	m_hhfFillPass
		->update("m_hhfResolution", m_hhfResolution)
		->getFrameBufferObject()->setFrameBufferObjectHandle(m_hhfMipmapFBOHandles[0]);
	m_resultPass
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
	delete m_hhfReducePass;
	delete m_hhfFillPass;
	delete m_resultPass;
	delete m_hudPass;
	delete m_panelPass;
}

void TelepresenceSession::updateProjectionMatrices(glm::mat4 projection)
{
	m_cubePass
		->update("projectionMatrix", projection);
	m_billboardPass
		->update("projectionMatrix", projection);
	m_hudPass
		->update("projectionMatrix", projection);
	m_panelPass
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
	m_hudPass
		->update("viewMatrix", glm::mat4(1.0f));
	m_panelPass
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

glm::vec3 TelepresenceSession::extractCameraPosition(glm::mat4 viewMatrix) const
{
	glm::mat3 rotMat(viewMatrix);
	glm::vec3 direction(viewMatrix[3]);
	return -direction * rotMat;
}

void TelepresenceSession::renderRoom(glm::vec3 cameraPosition)
{
	int intersected = 0;
	glm::mat4 leapToOculusTransformation = getLeapToOculusTransformationMatrix(cameraPosition);


	Leap::Hand leftHand = m_leapHandler->getLeftHand();
	Leap::Hand rightHand = m_leapHandler->getRightHand();

	//m_roomPass->run();
	for (unsigned int m = 0; m < m_assimpLoader->getMeshList()->size(); ++m)
	{
		Mesh* mesh = m_assimpLoader->getMeshList()->at(m);
		glm::mat4 modelMatrix = glm::rotate(m_assimpLoader->getModelMatrix(m), 0.0f, glm::vec3(0, 1, 0));
		m_roomShaders->update("modelMatrix", modelMatrix);
		m_roomShaders->update("diffuseColor", m_assimpLoader->getMaterialColor(m_assimpLoader->getMeshList()->at(m)->getMaterialIndex()));
		m_roomPass->getFrameBufferObject()->bind();
		m_roomPass->getShaderProgram()->use();
		m_assimpLoader->getMeshList()->at(m)->draw();
		// any purpose?!
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//if (leftHand.isValid() && rightHand.isValid() && m_leapHandler->isPinched(leftHand))
		//{
		//	vector<glm::vec3> ray;
		//	vector<glm::vec3> triangle;
		//	glm::vec3 intersectionPoint;
		//	glm::vec3 normal;

		//	glm::vec4 rayDirection = glm::vec4(m_leapHandler->convertLeapVecToGlm(rightHand.palmNormal()), 0);
		//	glm::mat4 leapWorldMatrixStart = getLeapWorldCoordinateMatrix(rightHand.palmPosition());
		//	glm::mat4 leapWorldMatrixDirection = getLeapWorldCoordinateMatrix(rightHand.palmNormal());

		//	glm::mat4 modelMatrixRay = leapToOculusTransformation * leapWorldMatrixStart;
		//	glm::mat4 modelMatrixRayDirection = leapToOculusTransformation * leapWorldMatrixDirection;
		//	glm::vec4 rayStart = modelMatrixRay * glm::vec4(m_leapHandler->convertLeapVecToGlm(rightHand.palmPosition()), 0);
		//	rayDirection = modelMatrixRayDirection * rayDirection;
		//	ray.push_back(glm::vec3(rayStart.x, rayStart.y, rayStart.z));
		//	ray.push_back(glm::vec3(rayDirection.x, rayDirection.y, rayDirection.z));

		//	std::vector<GLfloat>* vertices = mesh->getVertexList();
		//	std::vector<GLint>* indices = mesh->getIndexList();

		//	for (int i = 0; i < indices->size(); i++)
		//	{
		//		if (i > 0 && i % 3 == 0)
		//		{
		//			intersected = intersectionRayTriangle(ray, triangle, &intersectionPoint, &normal);
		//			triangle.clear();
		//			if (intersected == 1)
		//			{
		//				glm::quat rotationQuat = CameraObjectRelations::rotationBetweenVectors(m_textPanel->getNormal(), normal);
		//				glm::mat4 rotationMat = glm::toMat4(rotationQuat);
		//				glm::mat4 model = glm::translate(glm::translate(glm::mat4(1.0f), intersectionPoint), normal*.1f) * rotationMat;
		//				m_panelPass
		//					->update("modelMatrix", model)
		//					->texture("tex", m_textPanel->getTextureHandle())
		//					->run();
		//			}

		//		}
		//		int index = (indices->at(i) * 3);
		//		glm::vec3 vertex = glm::vec3(vertices->at(index), vertices->at(index + 1), vertices->at(index + 2));
		//		triangle.push_back(vertex);

		//	}
		//}
	}
}

void TelepresenceSession::renderPointCloud()
{
	m_pointCloud->updatePointCloud();
	m_pointCloudPass->run();
}

void TelepresenceSession::renderTestCube()
{
	glm::vec3 headPosition(0);
	for (auto body : m_pointCloud->getAllBodyJoints())
	{
		headPosition = glm::vec3(body[JointType_Head].Position.X, body[JointType_Head].Position.Y, body[JointType_Head].Position.Z);
	}

	m_cubePass
		->update("diffuseColor", glm::vec3(.0, 1.0, .0))
		->update("modelMatrix", glm::translate(glm::mat4(1.0f), headPosition))
		->run();
}

void TelepresenceSession::renderHud(glm::vec3 cameraPosition)
{
	// Measure speed
	double currentTime = glfwGetTime();
	nbFrames++;

	if (currentTime - lastTime >= 1.0){ // If last prinf() was more than 1 sec ago
		// printf and reset timer
		//string erg = std::to_string(1000.0 / double(nbFrames)) + "ms / frame\n";
		string erg = "FPS:" + std::to_string(double(nbFrames) / 1000.0f) + "\n lolo";
		m_hud->updateText(erg);
		nbFrames = 0;
		lastTime += 1.0;
	}

	/*int width, height;
	glfwGetWindowSize(m_window, &width, &height);

	printf("width: %d", width);
	printf("height: %d", height);*/

	m_hudPass
		->update("modelMatrix", glm::translate(glm::mat4(1.0f), glm::vec3(1, 1, -1)))
		->texture("tex", m_hud->getTextureHandle())
		->run();

}

void TelepresenceSession::renderBillboards(glm::vec3 cameraPosition)
{
	m_textPane->updateText("Ll L l");


	glm::vec3 headPosition(0);
	for (auto body : m_pointCloud->getAllBodyJoints())
	{
		headPosition = glm::vec3(body[JointType_Head].Position.X, body[JointType_Head].Position.Y, body[JointType_Head].Position.Z);
	}
	glm::vec3 billboardPosition = headPosition;
	billboardPosition.x = headPosition.x + 0.6f;


	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, billboardPosition);
	glm::mat4 billboardRotation = CameraObjectRelations::getBillboardRotationMatrix(cameraPosition, billboardPosition);
	modelMatrix = billboardRotation * modelMatrix;

	m_billboardPass
		->update("modelMatrix", billboardRotation)
		->texture("tex", m_textPane->getTextureHandle())
		->run();

}

void TelepresenceSession::renderPanels()
{
	m_panelPass
		->texture("tex", m_textPanel->getTextureHandle())
		->run();

}


void TelepresenceSession::renderLeap(glm::vec3 cameraPosition)
{
	Leap::Hand leftHand = m_leapHandler->getLeftHand();
	Leap::Hand rightHand = m_leapHandler->getRightHand();

	if (leftHand.isValid() || rightHand.isValid()){
		glm::mat4 leapToOculusTransformation = getLeapToOculusTransformationMatrix(cameraPosition);

		m_handPass
			->update("diffuseColor", glm::vec3(0.2f, 0.2f, 0.2f));

		for (auto joint : m_leapHandler->getJoints())
		{
			glm::mat4 leapWorldMatrix = getLeapWorldCoordinateMatrix(joint);
			glm::mat4 modelMatrix = leapToOculusTransformation * leapWorldMatrix;
			m_handPass
				->update("modelMatrix", modelMatrix)
				->run();
		}
		for (auto palmPosition : m_leapHandler->getPalmPositions())
		{
			glm::mat4 leapWorldMatrix = getLeapWorldCoordinateMatrix(palmPosition);
			glm::mat4 modelMatrix = leapToOculusTransformation * leapWorldMatrix;
			m_handPass
				->update("modelMatrix", modelMatrix)
				->run();
		}
		//render directionCube on rightHand if leftHand isPinched
		if (leftHand.isValid() && rightHand.isValid() && m_leapHandler->isPinched(leftHand)){
			Leap::Vector rightHandPalmPosition = rightHand.palmPosition();
			Leap::Matrix rightHandBasis = rightHand.basis();

			glm::mat4 leapWorldMatrix = getLeapWorldCoordinateMatrix(rightHandBasis, rightHandPalmPosition);
			glm::mat4 modelMatrix = leapToOculusTransformation * leapWorldMatrix;
			m_directionPass
				->update("modelMatrix", glm::translate(glm::scale(modelMatrix, glm::vec3(1.0, 500.0, 1.0)), glm::vec3(0.0, -2.0, 0.0)))
				->run();
		}
	}
}

void TelepresenceSession::renderHHF(){
	
	// set texture
	m_hhfReducePass->texture("m_pcOutputTex", m_hhfTexture);

	// reduce pass over all mipmap level
	for (m_hhfMipmapLevel = 0; m_hhfMipmapLevel < m_hhfMipmapNumber; m_hhfMipmapLevel++){
		m_hhfReducePass
			->update("m_hhfMipmapLevel", m_hhfMipmapLevel)
			->setFrameBufferObject(m_hhfMipmapFBOs[m_hhfMipmapLevel])
			->run();

		/*if (m_hhfMipmapLevel == 0){
			m_hhfReducePass
				->texture("m_pcOutputTex", m_hhfTexture);
		}*/
	}

	m_hhfFillPass
		->texture("m_hhfTexture", m_hhfTexture);
	
	// fill pass over all mipmap level
	for (m_hhfMipmapLevel = m_hhfMipmapNumber - 2; m_hhfMipmapLevel >= 0; m_hhfMipmapLevel--){
					
		m_hhfFillPass
			->texture("m_hhfTexture", m_hhfTexture)
			->update("m_hhfMipmapLevel", m_hhfMipmapLevel)
			->setFrameBufferObject(m_hhfMipmapFBOs[m_hhfMipmapLevel])		
			->run();	
	}

	// minimal fill pass for mipmap level 0 only - testing purpose
/*
	m_hhfFillPass
		->texture("m_hhfTexture", m_hhfTexture)
		->update("m_hhfMipmapLevel", 0)	
		->run();*/
}

void TelepresenceSession::renderResult(){

	m_resultPass
		->texture("m_resultTex", m_hhfTexture)
		->run();
}

glm::mat4 TelepresenceSession::getLeapToOculusTransformationMatrix(glm::vec3 cameraPosition) const
{
	//transform world coordinates into Oculus coordinates (for attached Leap Motion)
	ovrPosef headPose = ovrHmd_GetTrackingState(g_Hmd, 0.0f).HeadPose.ThePose;
	//glm::mat4 headTranslation = toGlm(OVR::Matrix4f::Translation(headPose.Position));	
	glm::mat4 headRotation = toGlm(OVR::Matrix4f(headPose.Orientation));
	glm::mat4 cameraTranslation = glm::translate(glm::mat4(1.0f), cameraPosition);
	glm::mat4 leapToOculusPipeline = cameraTranslation * headRotation * oculusToLeap * normalizeMat;
	return leapToOculusPipeline;
}

glm::mat4 TelepresenceSession::getLeapWorldCoordinateMatrix(const Leap::Vector &position) const
{
	glm::mat4 leapWorldCoordinates = glm::translate(glm::mat4(1.0f), m_leapHandler->convertLeapVecToGlm(position));
	return leapWorldCoordinates;
}

glm::mat4 TelepresenceSession::getLeapWorldCoordinateMatrix(const Leap::Matrix &basis, const Leap::Vector &position) const
{
	//compute rotation and translation of Leap Motion Bones
	glm::mat4 rotationMat = m_leapHandler->convertLeapMatToGlm(basis);
	glm::mat4 leapWorldCoordinates = glm::translate(glm::mat4(1.0f), m_leapHandler->convertLeapVecToGlm(position));
	leapWorldCoordinates = leapWorldCoordinates * rotationMat;
	//Pipeline for transforming Leap Motion bones
	return leapWorldCoordinates;
}


void TelepresenceSession::generateHoleFillingAssets(){
	m_hhfResolution = glm::vec2{ g_RenderTargetSize.w, g_RenderTargetSize.h };

	m_hhfVao = new Quad();
	m_hhfMipmapNumber = 4;

	glGenTextures(1, &m_hhfTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_hhfTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_hhfResolution.x, m_hhfResolution.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, m_hhfMipmapNumber-1);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glGenerateMipmap(GL_TEXTURE_2D);

	m_hhfMipmapFBOHandles = new GLuint[m_hhfMipmapNumber];
	m_hhfMipmapDepthHandles = new GLuint[m_hhfMipmapNumber];
	glGenFramebuffers(m_hhfMipmapNumber, m_hhfMipmapFBOHandles);
	glGenRenderbuffers(m_hhfMipmapNumber, m_hhfMipmapDepthHandles);
	for (int i = 0; i < m_hhfMipmapNumber; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, (m_hhfMipmapFBOHandles)[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_hhfTexture, i);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		// depth buffer for fbos
		// is this needed for point cloud stuff?
		// if activated texture m_hhfTexture is empty	
		/*glBindRenderbuffer(GL_RENDERBUFFER, (m_hhfMipmapDepthHandles)[i]);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, g_RenderTargetSize.w, g_RenderTargetSize.h);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_hhfMipmapDepthHandles[i]);
		*/
		FrameBufferObject* m_hhfFBO = new FrameBufferObject();
		m_hhfFBO->setFrameBufferObjectHandle(m_hhfMipmapFBOHandles[i]);
		m_hhfMipmapFBOs.push_back(m_hhfFBO);
			
		GLenum l_Check = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (l_Check != GL_FRAMEBUFFER_COMPLETE)
		{
			printf("There is a problem with the FBO %i.\n", i);
			exit(EXIT_FAILURE);
		}
		else {
			printf("HHF FBO %i is complete.\n", i);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	}
	/*glBindFramebuffer(GL_FRAMEBUFFER, (m_hhfMipmapFBOHandles)[0]);
	GLenum l_Check = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (l_Check != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("There is a problem with the FBO.\n");
		exit(EXIT_FAILURE);
	}
	else {
		printf("HHF FBO is complete.\n");
	}*/

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// texture with random data stuff
	/*texdata = (GLfloat *)malloc(g_RenderTargetSize.w*g_RenderTargetSize.h * 4 * sizeof(float));
	for (size_t i = 0; i<g_RenderTargetSize.w*g_RenderTargetSize.h * 4; i += 4){
	
		texdata[i] = ((float)rand() / (RAND_MAX)); // rand()%255;
		texdata[i + 1] = ((float)rand() / (RAND_MAX));
		texdata[i + 2] = ((float)rand() / (RAND_MAX));
		texdata[i + 3] = 1.0;
	}
	glGenTextures(1, &ttex);
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ttex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, m_hhfMipmapNumber-1);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, g_RenderTargetSize.w, g_RenderTargetSize.h, 0, GL_RGBA, GL_FLOAT, texdata);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);*/
}

// intersect3D_RayTriangle(): find the 3D intersection of a ray with a triangle
//    Input:  a ray R, and a triangle T
//    Output: *I = intersection point (when it exists)
//    Return: -1 = triangle is degenerate (a segment or point)
//             0 =  disjoint (no intersect)
//             1 =  intersect in unique point I1
//             2 =  are in the same plane
int TelepresenceSession::intersectionRayTriangle(std::vector<glm::vec3> ray, std::vector<glm::vec3> triangle, glm::vec3* intersectionPoint, glm::vec3* normal)
{
	glm::vec3    u, v, n;              // triangle vectors
	glm::vec3    dir, w0, w;           // ray vectors
	float     r, a, b;              // params to calc ray-plane intersect

	// get triangle edge vectors and plane normal
	u = triangle.at(1) - triangle.at(0);
	v = triangle.at(2) - triangle.at(0);
	n = glm::cross(u, v);              // cross product
	n = glm::normalize(n);
	*normal = n;
	if (n == glm::vec3(0))             // triangle is degenerate
		return -1;                  // do not deal with this case

	dir = ray.at(1);              // ray direction vector
	w0 = ray.at(0) - triangle.at(0);
	a = -glm::dot(n, w0);
	b = glm::dot(n, dir);
	if (fabs(b) < 0.00000001) {     // ray is  parallel to triangle plane
		if (a == 0)                 // ray lies in triangle plane
			return 2;
		else return 0;              // ray disjoint from plane
	}

	// get intersect point of ray with triangle plane
	r = a / b;
	if (r < 0.0)                    // ray goes away from triangle
		return 0;                   // => no intersect
	// for a segment, also test if (r > 1.0) => no intersect

	*intersectionPoint = ray.at(0) + r * dir;            // intersect point of ray and plane

	// is intersectionPoint inside triangle?
	float    uu, uv, vv, wu, wv, D;
	uu = glm::dot(u, u);
	uv = glm::dot(u, v);
	vv = glm::dot(v, v);
	w = *intersectionPoint - triangle.at(0);
	wu = glm::dot(w, u);
	wv = glm::dot(w, v);
	D = uv * uv - uu * vv;

	// get and test parametric coords
	float s, t;
	s = (uv * wv - vv * wu) / D;
	if (s < 0.0 || s > 1.0)         // intersectionPoint is outside triangle
		return 0;
	t = (uv * wu - uu * wv) / D;
	if (t < 0.0 || (s + t) > 1.0)  // intersectionPoint is outside triangle
		return 0;

	return 1;                       // intersectionPoint is in triangle
}