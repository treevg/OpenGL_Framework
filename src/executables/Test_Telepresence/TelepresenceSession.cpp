#include "TelepresenceSession.h"
#include "ShaderTools/OculusRenderer.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"
#include "ShaderTools/VertexArrayObjects/Sphere.h"
#include "LeapHandler.h"
#include "KinectHandler.h"
#include "AssimpLoader/AssimpLoader.h"
#include "TextPane.h"
#include "PointCloud.h"
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
	m_assimpLoader = new AssimpLoader();
	m_kinectHandler = new KinectHandler();
	m_leapHandler = new LeapHandler();
	m_kinectHandler->initializeDefaultSensor();
	m_pointCloud = new PointCloud( m_kinectHandler);
	m_kinectHandler->retrieveCameraIntrinsics();
	m_assimpLoader->loadFile(RESOURCES_PATH "/obj/room.obj")
		->printLog();
	m_hhfVao == NULL;

	generateHoleFillingAssets();

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
	
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (int i = 0; i < m_hhfMipmapNumber; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, (m_hhfMipmapFBOHandles)[i]);
		//glBindRenderbuffer(GL_RENDERBUFFER, (m_hhfMipmapDepthHandles)[i]);
		//glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glBindFramebuffer(GL_FRAMEBUFFER, (m_hhfMipmapFBOHandles)[0]);
	//glBindRenderbuffer(GL_RENDERBUFFER, (m_hhfMipmapDepthHandles)[0]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	updateProjectionMatrices(projection);
	updateViewMatrices(view);

	glm::vec3 cameraPosition = extractCameraPosition(view);

	m_pointCloud->updatePointCloud();
	renderBillboards(cameraPosition);
	renderRoom();
	renderTestCube();
	//renderLeap();
	// if point cloud AND hhf are activated point cloud can not be seen in hhf texture
	// strange
	renderPointCloud();
	renderHHF();
	//renderResult();
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
	//m_handShaders = new ShaderProgram({ "/Test_Telepresence/phong.vert", "/Test_Telepresence/phong.frag" });
	//m_pointCloudShaders = new ShaderProgram({ "/Test_Telepresence/minimal.vert", "/Test_Telepresence/minimal.frag" });
	//m_roomShaders = new ShaderProgram({ "/Test_Telepresence/minimalmat.vert", "/Test_Telepresence/minimalmat.frag" });
	//m_billboardShaders = new ShaderProgram({ "/Test_Telepresence/texture.vert", "/Test_Telepresence/texture.frag" });
	m_cubeShaders = new ShaderProgram({ "/Test_Telepresence/phong.vert", "/Test_Telepresence/phong.frag" });
	m_directionShaders = new ShaderProgram({ "/Test_Telepresence/phong.vert", "/Test_Telepresence/phong.frag" });
	m_handShaders = new ShaderProgram({ "/Test_Telepresence/phong.vert", "/Test_Telepresence/phong.frag" });
	m_pointCloudShaders = new ShaderProgram({ "/Test_Telepresence/minimal.vert", "/Test_Telepresence/minimal.frag" });
	m_roomShaders = new ShaderProgram({ "/Test_Telepresence/minimalmat.vert", "/Test_Telepresence/minimalmat.frag" });
	m_billboardShaders = new ShaderProgram({ "/Test_Telepresence/texture.vert", "/Test_Telepresence/texture.frag" });
	m_hhfReduceShaders = new ShaderProgram({ "/Test_Telepresence/hhf.vert", "/Test_Telepresence/reduce.frag" });
	m_hhfFillShaders = new ShaderProgram({ "/Test_Telepresence/hhf.vert", "/Test_Telepresence/fill.frag" });
	m_resultShaders = new ShaderProgram({ "/Test_Telepresence/hhf.vert", "/Test_Telepresence/fill.frag" });
}

void TelepresenceSession::initRenderPasses()
{
	Cube* testCube = new Cube(glm::vec3(1.0f, 0.0f, -4.0f), .5f);
	Cube* directionCube = new Cube(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
	Sphere* sphere = new Sphere(10.0f);
	Quad* quad = new Quad();
	
	m_textPane = new TextPane(glm::vec3(-2.0f, 0.0f, -3.0f), 2.0f, 1.0f, "Herzlich Willkommen");

	m_cubePass = new RenderPass(testCube, m_cubeShaders);
	m_billboardPass = new RenderPass(m_textPane, m_billboardShaders);
	m_handPass = new RenderPass( sphere, m_handShaders);
	m_roomPass = new RenderPass( m_assimpLoader->getMeshList()->at(0), m_roomShaders);
	m_pointCloudPass = new RenderPass(m_pointCloud, m_pointCloudShaders);
	m_directionPass = new RenderPass( directionCube, m_directionShaders);
	m_hhfReducePass = new RenderPass(m_hhfVao, m_hhfReduceShaders);
	m_hhfFillPass = new RenderPass(m_hhfVao, m_hhfFillShaders);
	m_resultPass = = new RenderPass(m_hhfVao, m_resultShaders);

	glm::vec3 lightPos = glm::vec3(2.0f, 10.0f, 2.0f);

	m_cubePass
		->update("lightPosition", lightPos)
		->getFrameBufferObject()->setFrameBufferObjectHandle(m_hhfMipmapFBOHandles[0]);
	m_billboardPass
		->getFrameBufferObject()->setFrameBufferObjectHandle(m_hhfMipmapFBOHandles[0]);
	m_handPass
		->update("lightPosition", lightPos)
		->getFrameBufferObject()->setFrameBufferObjectHandle(m_hhfMipmapFBOHandles[0]);
	m_roomPass
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
		// any purpose?!
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void TelepresenceSession::renderPointCloud()
{
	m_pointCloudPass
		->run();
	//glBindFramebuffer(GL_FRAMEBUFFER, 1);
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

void TelepresenceSession::renderHHF(){
	
	// set texture
	m_hhfReducePass->texture("m_pcOutputTex", m_hhfTexture);

	//// reduce pass over all mipmap level
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
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	for (m_hhfMipmapLevel = m_hhfMipmapNumber - 2; m_hhfMipmapLevel >= 0; m_hhfMipmapLevel--){
		//glBindRenderbuffer(GL_RENDERBUFFER, (m_hhfMipmapDepthHandles)[m_hhfMipmapLevel]);
		if (m_hhfMipmapLevel > 0){
			m_hhfFillPass
				->setFrameBufferObject(m_hhfMipmapFBOs[m_hhfMipmapLevel]);
			m_hhfFillPass
				//->texture("m_hhfTexture", m_hhfTexture)
				->update("m_hhfMipmapLevel", m_hhfMipmapLevel)
				->run();
		}
		else if (m_hhfMipmapLevel == 0){
			m_hhfFillPass
				->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);

			m_hhfFillPass
				//->texture("m_hhfTexture", m_hhfTexture)
				->update("m_hhfMipmapLevel", m_hhfMipmapLevel)
				->run();
		}
	}

	//glBindFramebuffer(GL_FRAMEBUFFER, 2);

	// minimal fill pass for mipmap level 0 only - testing purpose
/*
	m_hhfFillPass
		->texture("m_hhfTexture", m_hhfTexture)
		->update("m_hhfMipmapLevel", 0)	
		->run();*/
}

void TelepresenceSession::renderResult(){

	m_resultPass
		->texture("m_ResultTex", m_hhfTexture)
		->run();
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