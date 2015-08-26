#include <glm/gtc/matrix_transform.hpp>
#include <Leap.h>

class LeapHandler;
class GLFWwindow;
class AssimpLoader;
class KinectHandler;
class ShaderProgram;
class RenderPass;
class TextPane;
class PointCloud;

#pragma once

class TelepresenceSession
{
public:
	TelepresenceSession();
	~TelepresenceSession();

	void init();
	void run();
	void stop();

private:
	GLFWwindow* m_window;
	LeapHandler* m_leapHandler;
	AssimpLoader* m_assimpLoader;
	KinectHandler* m_kinectHandler;

	ShaderProgram* m_phongShaders;
	ShaderProgram* m_minimalShaders;
	ShaderProgram* m_minimalMatShaders;
	ShaderProgram* m_textureShaders;

	//TODO do not save this as member
	TextPane* m_textPane;
	PointCloud* m_pointCloud;

	RenderPass* m_roomPass;
	RenderPass* m_pointCloudPass;
	RenderPass* m_directionPass;
	RenderPass* m_handPass;
	RenderPass* m_cubePass;
	RenderPass* m_billboardPass;

	void renderLoop(double deltaTime, glm::mat4 projection, glm::mat4 view);

	void renderRoom();
	void renderPointCloud();
	void renderLeap();
	void renderBillboards(glm::vec3 cameraPosition);
	void renderTestCube();

	void generateOculusWindow();
	void initOpenGL();
	void initShaderPrograms();
	void initRenderPasses();
	void deleteShaderPrograms();

	void updateProjectionMatrices(glm::mat4 projection);
	void updateViewMatrices(glm::mat4 view);

	glm::vec3 extractCameraPosition(glm::mat4 viewMatrix) const;
	glm::mat4 getLeapToOculusTransformationMatrix() const;
	glm::mat4 getLeapWorldCoordinateMatrix(const Leap::Vector &position) const;
	glm::mat4 getLeapWorldCoordinateMatrix(const Leap::Matrix &basis, const Leap::Vector &position) const;
};