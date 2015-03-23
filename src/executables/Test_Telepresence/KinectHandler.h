#include <iostream>
#include <string.h>
#include "glm\glm.hpp"
#include "Kinect.h"
#include <windows.h>
#include "stdafx.h"
#include "GL\glew.h"

using namespace std;

class KinectHandler{
	static const int DepthWidth = 512;
	static const int DepthHeight = 424;

public:
	KinectHandler();
	
	// Initializes the default Kinect sensor
	// <returns>S_OK on success, otherwise failure code</returns>
	HRESULT	InitializeDefaultSensor();	

	/// Main processing function
	void Update(GLfloat *data);

private:
	// Current Kinect
	IKinectSensor* KinectSensor;
	
	// Depth reader
	IDepthFrameReader* DepthFrameReader;
	
	// MulitSourceFrameReader
	IMultiSourceFrameReader* MultiSourceFrameReader; 

	// Coordinate mapper to map one type of point to another
	ICoordinateMapper* coordinateMapper;

	// Intermediate storage for the depth to color mapping
	//ColorSpacePoint* colorPoints;
	//vector<ColorSpacePoint> colorPoints; 


	// Handle new depth data
	void ProcessDepth(INT64 nTime, const UINT16* pBuffer, int nHeight, int nWidth, USHORT nMinDepth, USHORT nMaxDepth, GLfloat *data);

};
