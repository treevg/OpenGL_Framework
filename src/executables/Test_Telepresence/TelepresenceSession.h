#include <glm/gtc/matrix_transform.hpp>
#include <Leap.h>
#include "ShaderTools\RenderPass.h"

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

	ShaderProgram* m_handShaders;
	ShaderProgram* m_directionShaders;
	ShaderProgram* m_cubeShaders;
	ShaderProgram* m_pointCloudShaders;
	ShaderProgram* m_roomShaders;
	ShaderProgram* m_billboardShaders;
	ShaderProgram* m_hhfReduceShaders;
	ShaderProgram* m_hhfFillShaders;
	ShaderProgram* m_hhfExpandShaders;

	//TODO do not save this as member
	TextPane* m_textPane;
	PointCloud* m_pointCloud;

	RenderPass* m_roomPass;
	RenderPass* m_pointCloudPass;
	RenderPass* m_directionPass;
	RenderPass* m_handPass;
	RenderPass* m_cubePass;
	RenderPass* m_billboardPass;
	RenderPass* m_hhfReducePass;
	RenderPass* m_hhfExpandPass;
	RenderPass* m_hhfFillPass;

	// Screen quad to render hhf image into
	VertexArrayObject* m_hhfVao;

	glm::vec2 m_hhfResolution;

	// hhf texture
	GLuint m_hhfTexture;
	// number of mipmaps
	GLuint m_hhfMipmapNumber;
	// mipmap fbo handle
	GLuint* m_hhfMipmapFBOHandles;
	// mipmap texture frame buffer
	std::vector<FrameBufferObject*> m_hhfMipmapFBOs;
	// current mipmap level
	int m_hhfMipmapLevel;
	// gaussian radius (shader)
	int m_hhfGaussianRadius;

	GLuint m_hhfMipmapDepthHandle;

	// Testkram
	GLfloat* texdata;
	GLuint ttex;

	//

	void renderLoop(double deltaTime, glm::mat4 projection, glm::mat4 view);

	void renderRoom();
	void renderPointCloud();
	void renderLeap();
	void renderBillboards(glm::vec3 cameraPosition);
	void renderTestCube();
	void renderHHF();

	void generateOculusWindow();
	void initOpenGL();
	void initShaderPrograms();
	void initRenderPasses();
	void deleteShaderPrograms();

	void updateProjectionMatrices(glm::mat4 projection);
	void updateViewMatrices(glm::mat4 view);

	void generateHoleFillingAssets();

	glm::vec3 extractCameraPosition(glm::mat4 viewMatrix) const;
	glm::mat4 getLeapToOculusTransformationMatrix() const;
	glm::mat4 getLeapWorldCoordinateMatrix(const Leap::Vector &position) const;
	glm::mat4 getLeapWorldCoordinateMatrix(const Leap::Matrix &basis, const Leap::Vector &position) const;
};