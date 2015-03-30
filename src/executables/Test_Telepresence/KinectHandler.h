#include <iostream>
#include <string.h>
#include "glm\glm.hpp"
#include "Kinect.h"
#include <windows.h>
#include "stdafx.h"
#include "GL\glew.h"
#include <vector>

using namespace std;

class KinectHandler{

public:
	KinectHandler();
	
	// Initializes the default Kinect sensor
	HRESULT	initializeDefaultSensor();	

	/// Main processing function
	void update(GLfloat *data);
	void update(GLfloat *positionData, GLfloat *colorData);

	void clearBuffer(GLfloat *buffer, int size); 

private:
	IKinectSensor* kinectSensor;
	IMultiSourceFrameReader* multiSourceFrameReader;
	IDepthFrame* depthFrame;
	IColorFrame* colorFrame;
	IBodyIndexFrame* bodyIndexFrame;
	IDepthFrameReference* depthReference;
	IColorFrameReference* colorReference;
	IBodyIndexFrameReference* bodyIndexReference;
	IFrameDescription* depthFrameDescription;
	IFrameDescription* colorFrameDescription;
	ICoordinateMapper* coordinateMapper; // Coordinate mapper to map one type of point to another

	// Intermediate storage for the depth to color mapping
	ColorSpacePoint* colorPoints;
	CameraSpacePoint* cameraPoints;

	// To Reserve Depth Frame Buffer
	UINT16* depthBuffer;
	// To Reserve Color Frame Buffer
	RGBQUAD* colorBuffer;
	// To Reserve Body Index Frame Buffer
	byte* bodyIndexBuffer;
};
