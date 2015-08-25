#include "../../libraries/ShaderTools/VertexArrayObject.h"

#pragma once

class KinectHandler;

class PointCloud : public VertexArrayObject {
public:
	PointCloud( const KinectHandler* kinectHandler );
	void draw();
	void updatePointCloud(GLfloat *positionData, GLfloat *colorData);
	void deleteBuffers();

private:
	GLuint vertexBufferHandles[2];
	const KinectHandler* m_kinectHandler;
};