#include "PointCloud.h"
#include "KinectHandler.h"

PointCloud::PointCloud(KinectHandler* kinectHandler)
{
	mode = GL_POINTS;

	glGenVertexArrays(1, &vertexArrayObjectHandle);
	glBindVertexArray(vertexArrayObjectHandle);

	glGenBuffers(2, vertexBufferHandles);

	m_kinectHandler = kinectHandler;
}

void PointCloud::draw() {

	glBindVertexArray(vertexArrayObjectHandle);
	glPointSize(3.0f);
	glDrawArrays(mode, 0, depthWidth * depthHeight);
	glBindVertexArray(0);
}

void PointCloud::updatePointCloud(){

	if (m_kinectHandler->updateKinect(m_colorData, m_positionData) )
	{
		glBindVertexArray(vertexArrayObjectHandle);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[0]);
		glBufferData(GL_ARRAY_BUFFER, depthWidth * depthHeight * 3 * sizeof(float), m_positionData, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[1]);
		glBufferData(GL_ARRAY_BUFFER, depthWidth * depthHeight * 3 * sizeof(float), m_colorData, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}/*
	else{
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[0]);
		glBufferData(GL_ARRAY_BUFFER, depthWidth * depthHeight * 3 * sizeof(float), m_positionData, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(4);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[1]);
		glBufferData(GL_ARRAY_BUFFER, depthWidth * depthHeight * 3 * sizeof(float), m_colorData, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(5);
	}*/
}

std::vector<std::vector<Joint>> PointCloud::getAllBodyJoints()
{
	return m_kinectHandler->getBodyJoints();
}