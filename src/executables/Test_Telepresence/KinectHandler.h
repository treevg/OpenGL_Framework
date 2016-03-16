#include <iostream>
#include "Kinect.h"
#include <windows.h>
#include "GL\glew.h"
#include <vector>
#include <glm/gtx/associated_min_max.hpp>

using namespace std;

class KinectHandler{

public:
	KinectHandler();
	~KinectHandler();

	// Initializes the default Kinect sensor
	HRESULT	initializeDefaultSensor();
	void getDepthFrameDescription(HRESULT& hr, int& depthWidth, int& depthHeight);
	void retrieveColorPoints(GLfloat* colorData, GLfloat* positionData, HRESULT& hr, int depthWidth, int depthHeight, int colorWidth, int colorHeight);
	void fillBuffers(GLfloat* colorData, GLfloat* positionData, int depthWidth, int depthHeight, int colorWidth, int colorHeight);
	std::vector<std::vector<Joint>> getBodyData(IMultiSourceFrame* multiSourceFrame) const;
	int calculateCollision(glm::vec3 start, glm::vec3 direction, IBody* bodies);
	// Main processing function
	//void updateKinect(GLfloat *data);
	bool updateKinect( GLfloat* colorData, GLfloat* positionData );
	
	std::vector<std::vector<Joint>> getBodyJoints();

	void retrieveCameraIntrinsics();
	void printCameraIntrinsics(CameraIntrinsics* cI);
	void clearBuffer(GLfloat *buffer, int size);

private:
	IKinectSensor* kinectSensor;
	IMultiSourceFrameReader* multiSourceFrameReader;
	IDepthFrame* depthFrame;
	IColorFrame* colorFrame;
	IBodyIndexFrame* bodyIndexFrame;
	IBodyFrame* bodyFrame;
	IDepthFrameReference* depthReference;
	IColorFrameReference* colorReference;
	IBodyFrameReference* bodyFrameReference;
	IBodyIndexFrameReference* bodyIndexReference;
	IFrameDescription* depthFrameDescription;
	IFrameDescription* colorFrameDescription;
	ICoordinateMapper* coordinateMapper; // Coordinate mapper to map one type of point to another
	
	CameraIntrinsics* cameraIntrinsics;
	WAITABLE_HANDLE m_coordinateMappingChangedEvent;

	// Intermediate storage for the depth to color mapping
	ColorSpacePoint* colorPoints;
	CameraSpacePoint* cameraPoints;

	// To Reserve Depth Frame Buffer
	UINT16* depthBuffer;
	// To Reserve Color Frame Buffer
	RGBQUAD* colorBuffer;
	// To Reserve Body Index Frame Buffer
	byte* bodyIndexBuffer;

	// joint data (max size is BODY_COUNT (6))
	std::vector<std::vector<Joint>> m_jointPositions;

	TIMESPAN m_latestTimeStamp = 0;
};
