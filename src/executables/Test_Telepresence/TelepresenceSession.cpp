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
	std::cout << "Init Session" << std::endl;
	generateOculusWindow();
	initOpenGL();
	m_assimpLoader = new AssimpLoader();
	m_kinectHandler = new KinectHandler();
	m_leapHandler = new LeapHandler();
	m_kinectHandler->initializeDefaultSensor();
	m_pointCloud = new PointCloud( m_kinectHandler);
	m_kinectHandler->retrieveCameraIntrinsics();
	m_assimpLoader->loadFile(RESOURCES_PATH "/obj/room.obj")
		->printLog();
	initShaderPrograms();
	initRenderPasses();
	
}

void TelepresenceSession::run()
{
	render(m_window, [&]( double delta, glm::mat4 projection, glm::mat4 view)
	{
		//m_kinectHandler->retrieveCameraIntrinsics();
		renderLoop(delta, projection, view);
		
	});
}

void TelepresenceSession::renderLoop(double deltaTime, glm::mat4 projection, glm::mat4 view)
{
	updateProjectionMatrices(projection);
	updateViewMatrices(view);

	glm::vec3 cameraPosition = extractCameraPosition(view);

	m_pointCloud->updatePointCloud();
	renderBillboards(cameraPosition);
	renderRoom();
	renderTestCube();
	renderLeap();
	renderPointCloud();

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
	m_handShaders = new ShaderProgram({ "/Test_Telepresence/phong.vert", "/Test_Telepresence/phong.frag" });
	m_pointCloudShaders = new ShaderProgram({ "/Test_Telepresence/minimal.vert", "/Test_Telepresence/minimal.frag" });
	m_roomShaders = new ShaderProgram({ "/Test_Telepresence/minimalmat.vert", "/Test_Telepresence/minimalmat.frag" });
	m_billboardShaders = new ShaderProgram({ "/Test_Telepresence/texture.vert", "/Test_Telepresence/texture.frag" });
	m_cubeShaders = new ShaderProgram({ "/Test_Telepresence/phong.vert", "/Test_Telepresence/phong.frag" });
	m_directionShaders = new ShaderProgram({ "/Test_Telepresence/phong.vert", "/Test_Telepresence/phong.frag" });
	m_handShaders = new ShaderProgram({ "/Test_Telepresence/phong.vert", "/Test_Telepresence/phong.frag" });
	m_pointCloudShaders = new ShaderProgram({ "/Test_Telepresence/minimal.vert", "/Test_Telepresence/minimal.frag" });
	m_roomShaders = new ShaderProgram({ "/Test_Telepresence/minimalmat.vert", "/Test_Telepresence/minimalmat.frag" });
	m_billboardShaders = new ShaderProgram({ "/Test_Telepresence/texture.vert", "/Test_Telepresence/texture.frag" });
}

void TelepresenceSession::initRenderPasses()
{
	Cube* testCube = new Cube(glm::vec3(1.0f, 0.0f, -4.0f), .5f);
	Cube* directionCube = new Cube(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
	Sphere* sphere = new Sphere(10.0f);
	m_textPane = new TextPane(glm::vec3(-2.0f, 0.0f, -3.0f), 2.0f, 1.0f, "Herzlich Willkommen");

	m_cubePass = new RenderPass(testCube, m_cubeShaders);
	m_billboardPass = new RenderPass(m_textPane, m_billboardShaders);
	m_handPass = new RenderPass( sphere, m_handShaders);
	m_roomPass = new RenderPass( m_assimpLoader->getMeshList()->at(0), m_roomShaders);
	m_pointCloudPass = new RenderPass(m_pointCloud, m_pointCloudShaders);
	m_directionPass = new RenderPass( directionCube, m_directionShaders);

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
		->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
	m_pointCloudPass
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

glm::vec3 TelepresenceSession::extractCameraPosition(glm::mat4 viewMatrix) const
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
		m_roomShaders->update("model", model);
		m_roomShaders->update("materialColor", m_assimpLoader->getMaterialColor(m_assimpLoader->getMeshList()->at(m)->getMaterialIndex()));
		m_roomPass->getFrameBufferObject()->bind();
		m_roomPass->getShaderProgram()->use();
		m_assimpLoader->getMeshList()->at(m)->draw();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void TelepresenceSession::renderPointCloud()
{
	//m_pointCloud->updatePointCloud();
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
	glm::mat4 leapToOculusTransformation = getLeapToOculusTransformationMatrix();

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
	for ( auto palmPosition : m_leapHandler->getPalmPositions())
	{
		glm::mat4 leapWorldMatrix = getLeapWorldCoordinateMatrix(palmPosition);
		glm::mat4 modelMatrix = leapToOculusTransformation * leapWorldMatrix;
		m_handPass
			->update("modelMatrix", modelMatrix)
			->run();
	}
}

glm::mat4 TelepresenceSession::getLeapToOculusTransformationMatrix() const
{
	//transform world coordinates into Oculus coordinates (for attached Leap Motion)
	ovrPosef headPose = ovrHmd_GetTrackingState(g_Hmd, 0.0f).HeadPose.ThePose;
	glm::mat4 headTranslation = toGlm(OVR::Matrix4f::Translation(headPose.Position));
	glm::mat4 headRotation = toGlm(OVR::Matrix4f(headPose.Orientation));
	glm::mat4 leapToOculusPipeline = headTranslation * headRotation * oculusToLeap * normalizeMat;
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
	glm::mat4 leapWorldCoordinates = glm::translate(rotationMat, m_leapHandler->convertLeapVecToGlm(position));

	//Pipeline for transforming Leap Motion bones
	return leapWorldCoordinates;
}
