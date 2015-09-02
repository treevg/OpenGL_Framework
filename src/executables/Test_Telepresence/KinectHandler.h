#include <iostream>
#include "Kinect.h"
#include <windows.h>
#include "GL\glew.h"

using namespace std;

class KinectHandler{

public:
	KinectHandler();
	
	// Initializes the default Kinect sensor
	HRESULT	initializeDefaultSensor();	

	// Main processing function
	void updateKinect(GLfloat *data);
	bool updateKinect( GLfloat* colorData, GLfloat* positionData );

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

	TIMESPAN m_latestTimeStamp = 0;
};
