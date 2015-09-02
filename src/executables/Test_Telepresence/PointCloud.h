#pragma once
#include "../../libraries/ShaderTools/VertexArrayObject.h"

class KinectHandler;

class PointCloud : public VertexArrayObject {
public:
	PointCloud( KinectHandler* kinectHandler);
	void draw();
	void updatePointCloud();

private:
	static const int depthWidth = 512;
	static const int depthHeight = 424;
	GLfloat* m_colorData = new float[depthWidth * depthHeight * 3];
	GLfloat* m_positionData = new float[depthWidth * depthHeight * 3];
	KinectHandler* m_kinectHandler;
	GLuint vertexBufferHandles[2];
};
