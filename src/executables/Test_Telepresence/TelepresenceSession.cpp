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
	initMouseAndKeyboardMovement();
	initFramesCounter();
	m_assimpLoader = new AssimpLoader();
	m_kinectHandler = new KinectHandler();
	m_leapHandler = new LeapHandler();
	m_kinectHandler->initializeDefaultSensor();
	m_pointCloud = new PointCloud(m_kinectHandler);
	m_assimpLoader->loadFile(RESOURCES_PATH "/obj/room2_tris.obj")
		->printLog();
	initShaderPrograms();
	initRenderPasses();
}

void TelepresenceSession::run()
{
	render(m_window, [&](double delta, glm::mat4 projection, glm::mat4 view)
	{
		renderLoop(delta, projection, view);
	});
}

void TelepresenceSession::renderLoop(double deltaTime, glm::mat4 projection, glm::mat4 view)
{
	measureSpeedOfApplication();

	computeMatricesFromInputs(m_window);
	//projection = getProjectionMatrix();
	view = getViewMatrix();

	updateProjectionMatrices(projection);
	updateViewMatrices(view);

	glm::vec3 cameraPosition = extractCameraPosition(view);

	renderBillboards(cameraPosition);
	//renderPanels();
	renderRoom(cameraPosition);
	renderTestCube();
	//renderLeap(cameraPosition);

	renderPointCloud();

	glDepthFunc(GL_ALWAYS);
	renderHud(cameraPosition);
	glDepthFunc(GL_LESS);


}

void TelepresenceSession::generateOculusWindow()
{
	m_window = generateWindow();
}

void TelepresenceSession::initMouseAndKeyboardMovement()
{
	// Hide the mouse and enable unlimited movement
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Keyboard Callback Method
	//glfwSetKeyCallback(m_window, keyboardInputs);

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
	m_cubeShaders = new ShaderProgram({ "/Test_Telepresence/phong.vert", "/Test_Telepresence/phong.frag" });
	m_directionShaders = new ShaderProgram({ "/Test_Telepresence/phong.vert", "/Test_Telepresence/phong.frag" });
	m_handShaders = new ShaderProgram({ "/Test_Telepresence/phong.vert", "/Test_Telepresence/phong.frag" });
	m_pointCloudShaders = new ShaderProgram({ "/Test_Telepresence/minimal.vert", "/Test_Telepresence/minimal.frag" });
	m_roomShaders = new ShaderProgram({ "/Test_Telepresence/phong.vert", "/Test_Telepresence/phong.frag" });
	m_panelShaders = new ShaderProgram({ "/Test_Telepresence/texture.vert", "/Test_Telepresence/texture.frag" });
}

void TelepresenceSession::initRenderPasses()
{
	Cube* testCube = new Cube(glm::vec3(0.0f, 0.0f, 0.0f), .1f);
	Cube* directionCube = new Cube(glm::vec3(0.0f, 0.0f, 0.0f), 2.0f);
	Sphere* sphere = new Sphere(10.0f);
	/*m_textPane = new TextPane(0.8f, .4f, "Matthias");*/
	m_hud = new TextPane(0.8f, .4f, "", 20);
	m_textPanel = new TextPane(2.0f, 1.0f, "Wand");
	m_cubePass = new RenderPass(testCube, m_cubeShaders);
	
	for (int i = 0; i < 6; ++i)
	{
		std::string text = "Person" + (i+1);
		TextPane* textPane = new TextPane(0.8f, .4f + (i * 20.0f), text, 30);
		RenderPass* billboardPass = new RenderPass(textPane, new ShaderProgram({ "/Test_Telepresence/texture.vert", "/Test_Telepresence/texture.frag" }));
		m_billboardPasses.push_back(std::make_pair(billboardPass, textPane));
	}

	m_hudPass = new RenderPass(m_hud, new ShaderProgram({ "/Test_Telepresence/texture.vert", "/Test_Telepresence/texture.frag" }));
	m_panelPass = new RenderPass(m_textPanel, m_panelShaders);
	m_handPass = new RenderPass(sphere, m_handShaders);
	m_roomPass = new RenderPass(m_assimpLoader->getMeshList()->at(0), m_roomShaders);
	m_pointCloudPass = new RenderPass(m_pointCloud, m_pointCloudShaders);
	m_directionPass = new RenderPass(directionCube, m_directionShaders);

	glm::vec3 lightPos = glm::vec3(0.0f, 4.0f, 2.0f);

	m_cubePass
		->update("lightPosition", lightPos)
		->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
	for (auto pass : m_billboardPasses)
	{
		pass.first
			->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
	}
	//m_billboardPass
	//	->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
	m_hudPass
		->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
	m_panelPass
		->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
	m_handPass
		->update("lightPosition", lightPos)
		->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
	m_roomPass
		->update("lightPosition", lightPos)
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
	for ( auto pass : m_billboardPasses )
	{
		delete pass.first;
	}
	//delete m_billboardPass;
	delete m_hudPass;
	delete m_panelPass;
}

void TelepresenceSession::updateProjectionMatrices(glm::mat4 projection)
{
	m_cubePass
		->update("projectionMatrix", projection);
	for (auto pass : m_billboardPasses)
	{
		pass.first
			->update("projectionMatrix", projection);
	}
	//m_billboardPass
	//	->update("projectionMatrix", projection);
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
	for (auto pass : m_billboardPasses)
	{
		pass.first
		->update("viewMatrix", view);
	}
	//m_billboardPass
	//	->update("viewMatrix", view);
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
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
		string erg = "FPS:" + std::to_string(double(nbFrames)/ 1000.0f) + "\n lolo";
		m_hud->updateText(erg);
		nbFrames = 0;
		lastTime += 1.0;
	}

	/*int width, height;
	glfwGetWindowSize(m_window, &width, &height);

	printf("width: %d", width);
	printf("height: %d", height);*/

	m_hudPass
		->update("modelMatrix", glm::translate(glm::mat4(1.0f), glm::vec3(1,1,-1)))
		->texture("tex", m_hud->getTextureHandle())
		->run();

}

void TelepresenceSession::renderBillboards(glm::vec3 cameraPosition)
{
	glm::vec3 headPosition(0);
	std::vector< glm::vec3 > billboardPositions;
	for (auto body : m_pointCloud->getAllBodyJoints())
	{
		headPosition = glm::vec3(body[JointType_Head].Position.X, body[JointType_Head].Position.Y, body[JointType_Head].Position.Z);
		glm::vec3 boardPosition = headPosition;
		boardPosition.x = boardPosition.x + 0.6f;
		billboardPositions.push_back(boardPosition);
	}

	int count = 0;
	for (auto billboardPosition : billboardPositions)
	{
		glm::mat4 billboardRotation = CameraObjectRelations::getBillboardRotationMatrix(cameraPosition, billboardPosition);

		auto passAndPane = m_billboardPasses[count];
		passAndPane.first
			->update("modelMatrix", billboardRotation)
			->texture("tex", passAndPane.second->getTextureHandle())
			->run();
		++count;
	}
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