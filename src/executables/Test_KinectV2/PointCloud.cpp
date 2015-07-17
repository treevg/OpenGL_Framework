#include "PointCloud.h"

PointCloud::PointCloud() {
	mode = GL_POINTS;

	glGenVertexArrays(1, &vertexArrayObjectHandle);
	glBindVertexArray(vertexArrayObjectHandle);

	glGenBuffers(2, vertexBufferHandles);
}

void PointCloud::draw() {
	glBindVertexArray(vertexArrayObjectHandle);
	glPointSize(2.0f);
	glDrawArrays(mode, 0, 512 * 424);
}

void PointCloud::updatePointCloud(GLfloat *positionData, GLfloat *colorData){
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[0]);
	glBufferData(GL_ARRAY_BUFFER, 512 * 424 * 3 * sizeof(float), positionData, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[1]);
	glBufferData(GL_ARRAY_BUFFER, 512 * 424 * 3 * sizeof(float), colorData, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

//void PointCloud::deleteBuffers(){
//	glDeleteBuffers(2, vertexBufferHandles);
//}