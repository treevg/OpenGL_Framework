#include <glm/gtc/matrix_transform.hpp>
#include <Leap.h>
#include "KeyCallbackWrapper.h"

class LeapHandler;
class GLFWwindow;
class AssimpLoader;
class KinectHandler;
class ShaderProgram;
class RenderPass;
class TextPane;
class PointCloud;

#pragma once

class TelepresenceSession : public KeyCallbackWrapper
{
public:
	TelepresenceSession();
	~TelepresenceSession();

	void init();
	void run();
	void stop();
	//void keyBoardInputs(GLFWwindow* p_Window, int p_Key, int scancode, int p_Action, int mods);
	virtual void keycallback(
		GLFWwindow *window,
		int key,
		int scancode,
		int action,
		int mods);
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
	ShaderProgram* m_hudShaders;
	ShaderProgram* m_panelShaders;

	//TODO do not save this as member
	TextPane* m_textPane;
	TextPane* m_hud;
	TextPane* m_textPanel;
	PointCloud* m_pointCloud;

	RenderPass* m_roomPass;
	RenderPass* m_pointCloudPass;
	RenderPass* m_directionPass;
	RenderPass* m_handPass;
	RenderPass* m_cubePass;
	RenderPass* m_billboardPass;
	RenderPass* m_hudPass;
	RenderPass* m_panelPass;

	// Keyboard Input Controls
	bool m_toggle_hud = true;
	bool m_toggle_pointcloud = true;
	bool m_toggle_userInfo = true;
	bool m_toggle_presentationCanvas = true;
	bool m_toggle_leapMotion = true;
	bool m_toggle_mouseAsCamera = true;
	bool m_toggle_mouseCursor = true;

	double lastTime;
	int nbFrames;

	void renderLoop(double deltaTime, glm::mat4 projection, glm::mat4 view);

	void renderRoom(glm::vec3 cameraPosition);
	void renderPointCloud();
	void renderLeap(glm::vec3 cameraPosition);
	void renderBillboards(glm::vec3 cameraPosition);
	void renderHud(glm::vec3 cameraPosition);
	void renderPanels();
	void renderTestCube();

	void generateOculusWindow();
	void initOpenGL();
	void initMouseAndKeyboardMovement();
	void initShaderPrograms();
	void initRenderPasses();
	void deleteShaderPrograms();

	void updateProjectionMatrices(glm::mat4 projection);
	void updateViewMatrices(glm::mat4 view);

	void initFramesCounter();
	void measureSpeedOfApplication();


	glm::vec3 extractCameraPosition(glm::mat4 viewMatrix) const;
	glm::mat4 getLeapToOculusTransformationMatrix(glm::vec3 cameraPosition) const;
	glm::mat4 getLeapWorldCoordinateMatrix(const Leap::Vector &position) const;
	glm::mat4 getLeapWorldCoordinateMatrix(const Leap::Matrix &basis, const Leap::Vector &position) const;

	int intersectionRayTriangle(std::vector<glm::vec3> ray, std::vector<glm::vec3> triangle, glm::vec3* intersectionPoint, glm::vec3* normal);
};