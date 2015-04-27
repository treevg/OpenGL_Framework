#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include "../../libraries/ShaderTools/VertexArrayObject.h"

class PointCloud : public VertexArrayObject {
public:
	PointCloud();
	void draw();
	void updatePointCloud(GLfloat *positionData, GLfloat *colorData);
	void deleteBuffers();

private:
	GLuint vertexBufferHandles[2];
};

#endif // POINTCLOUD_H
