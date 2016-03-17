#include "KinectHandler.h"
#include "stdafx.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

using namespace std;


KinectHandler::KinectHandler() :
kinectSensor(NULL),
multiSourceFrameReader(NULL),
depthFrame(NULL),
colorFrame(NULL),
bodyIndexFrame(NULL),
depthReference(NULL),
colorReference(NULL),
bodyIndexReference(NULL),
depthFrameDescription(NULL),
colorFrameDescription(NULL),
m_coordinateMappingChangedEvent(NULL),
coordinateMapper(nullptr),
cameraIntrinsics(NULL),
colorPoints(NULL),
cameraPoints(NULL),
depthBuffer(NULL),
colorBuffer(NULL),
bodyIndexBuffer(NULL),
bodyFrameReference(NULL),
bodyFrame(NULL)
{
}

KinectHandler::~KinectHandler()
{

}

bool KinectHandler::isKinectAvailable()
{
	return isAvailable;
}

HRESULT KinectHandler::initializeDefaultSensor()
{
	// returns S_OK on success, otherwise failure code
	HRESULT hr;

	hr = GetDefaultKinectSensor(&kinectSensor);
	if (FAILED(hr))
	{
		return hr;
	}
	cout << "kinectSensor " << kinectSensor << endl;
	
	if (kinectSensor)
	{
		// Initialize the Kinect
		hr = kinectSensor->Open();

		kinectSensor->get_CoordinateMapper(&coordinateMapper);
		coordinateMapper->SubscribeCoordinateMappingChanged(&m_coordinateMappingChangedEvent);
		//CameraIntrinsics* cameraIntrinsics = new CameraIntrinsics();

		if (SUCCEEDED(hr))
		{
			// open multiframereader for depth and color
			hr = kinectSensor->OpenMultiSourceFrameReader(FrameSourceTypes_Depth | FrameSourceTypes_Color | FrameSourceTypes_BodyIndex | FrameSourceTypes_Body, &multiSourceFrameReader);
		}
	}

	// Some Machines will only be detected after some sleep time: known kinect issue
	Sleep(1000);
	kinectSensor->get_IsAvailable(&isAvailable);


	if (!kinectSensor || FAILED(hr) || !isAvailable)
	{
		//SetStatusMessage(L"No ready Kinect found!", 10000, true);
		cout << "No ready Kinect found!" << endl;
		return E_FAIL;
	}

	return hr;
}

void KinectHandler::getDepthFrameDescription(HRESULT& hr, int& depthWidth, int& depthHeight)
{
	hr = depthFrame->get_FrameDescription(&depthFrameDescription);

	if (SUCCEEDED(hr))
	{
		hr = depthFrameDescription->get_Width(&depthWidth);
	}

	if (SUCCEEDED(hr))
	{
		hr = depthFrameDescription->get_Height(&depthHeight);
	}

	SafeRelease(depthFrameDescription);
}

// Main processing function
bool KinectHandler::updateKinect(GLfloat* colorData, GLfloat* positionData)
{
	//m_colorData = new float[depthWidth * depthHeight * 3];
	//m_positionData = new float[depthWidth * depthHeight * 3];

	IMultiSourceFrame* pMultiFrame = NULL;
	HRESULT hr = multiSourceFrameReader->AcquireLatestFrame(&pMultiFrame);

	// check if kinect is available
	kinectSensor->get_IsAvailable(&isAvailable);
	if (SUCCEEDED(hr))
	{
		int Width = 0;
		int Height = 0;
		int depthWidth = 0;
		int depthHeight = 0;
		int colorWidth = 0;
		int colorHeight = 0;
		USHORT minReliableDistance = 0;
		USHORT maxReliableDistance = 0;

		// ------------------------------ Get Depth Data ------------------------------ //
		hr = pMultiFrame->get_DepthFrameReference(&depthReference);
		TIMESPAN currentTimeStamp;
		depthReference->get_RelativeTime(&currentTimeStamp);
		if ( m_latestTimeStamp == currentTimeStamp )
		{
			return false;
		}
		m_latestTimeStamp = currentTimeStamp;

		depthFrame = nullptr;

		if (SUCCEEDED(hr))
		{
			hr = depthReference->AcquireFrame(&depthFrame);

			if (SUCCEEDED(hr))
			{
				getDepthFrameDescription(hr, depthWidth, depthHeight);

				if (SUCCEEDED(hr))
				{
					depthBuffer = new UINT16[depthWidth * depthHeight];

					// Update the depth data
					hr = depthFrame->CopyFrameDataToArray(depthWidth * depthHeight, &depthBuffer[0]);
			
					if (SUCCEEDED(hr))
					{
						hr = depthFrame->get_DepthMinReliableDistance(&minReliableDistance);
						//std::cout << "Min reliable depth: " << minReliableDistance << std::endl;
					}
						
					if (SUCCEEDED(hr))	
					{
						hr = depthFrame->get_DepthMaxReliableDistance(&maxReliableDistance);
						//std::cout << "Max reliable depth" << maxReliableDistance << std::endl;
					}

					if (SUCCEEDED(hr))
					{
						SafeRelease(depthFrame);

						// ------------------------------ Get Color Data ------------------------------ //
						hr = pMultiFrame->get_ColorFrameReference(&colorReference);

						if (SUCCEEDED(hr))
						{
							colorFrame = nullptr;
							hr = colorReference->AcquireFrame(&colorFrame);

							if (SUCCEEDED(hr))
							{
								hr = colorFrame->get_FrameDescription(&colorFrameDescription);

								if (SUCCEEDED(hr))
								{
									hr = colorFrameDescription->get_Width(&colorWidth);
								}

								if (SUCCEEDED(hr))
								{
									hr = colorFrameDescription->get_Height(&colorHeight);
								}

								SafeRelease(colorFrameDescription);
								
								if (SUCCEEDED(hr))
								{
									colorBuffer = new RGBQUAD[colorWidth * colorHeight];
								
									if (SUCCEEDED(hr))
									{
										ColorImageFormat format;
										hr = colorFrame->get_RawColorImageFormat(&format);

										if (SUCCEEDED(hr))
										{
											// Update the color data
											if (format == ColorImageFormat_Bgra)
												hr = colorFrame->CopyRawFrameDataToArray(colorWidth * colorHeight * sizeof(RGBQUAD), reinterpret_cast<BYTE*>(&colorBuffer[0]));
											else
												hr = colorFrame->CopyConvertedFrameDataToArray(colorWidth * colorHeight * sizeof(RGBQUAD), reinterpret_cast<BYTE*>(&colorBuffer[0]), ColorImageFormat_Bgra);
										}
										SafeRelease(colorFrame);



										// ------------------------------ Get Body Index Data ------------------------------ //
										hr = pMultiFrame->get_BodyIndexFrameReference(&bodyIndexReference);

										bodyIndexFrame = nullptr;

										if (SUCCEEDED(hr))
										{
											hr = bodyIndexReference->AcquireFrame(&bodyIndexFrame);
											
											bodyIndexBuffer = new byte[depthWidth * depthHeight];
											if (SUCCEEDED(hr))
											{
												// Update the body index data
												hr = bodyIndexFrame->CopyFrameDataToArray(depthWidth * depthHeight, &bodyIndexBuffer[0]);
											}

											retrieveColorPoints(colorData, positionData, hr, depthWidth, depthHeight, colorWidth, colorHeight);
											
											SafeRelease(bodyIndexFrame);

											m_jointPositions = getBodyData(pMultiFrame);

											//converted color
											delete[] bodyIndexBuffer;
										}//bodyindexbuffer

										SafeRelease(pMultiFrame);

									}//colorbuffer
									delete[] colorBuffer;
								}//framedescription color width u height
							}//colorframe
						}//colorreference
					}//depthbuffer
					delete[] depthBuffer;
				}//framedescription depth width u height
			}//dephtframe
		}//dephtframereference
	}//multiframe
	return true;
}

std::vector<std::vector<Joint>> KinectHandler::getBodyData(IMultiSourceFrame* frame) const
{
	// Body tracking variables
	BOOLEAN tracked;                            // Whether we see a body
	Joint joints[JointType_Count];              // List of joints in the tracked body

	std::vector<std::vector<Joint>> jointsVector;


	IBodyFrame* bodyframe;
	IBodyFrameReference* frameref = NULL;
	frame->get_BodyFrameReference(&frameref);
	frameref->AcquireFrame(&bodyframe);
	if (frameref) frameref->Release();

	if (!bodyframe) return std::vector<std::vector<Joint>>();

	// ------ NEW CODE ------
	IBody* body[BODY_COUNT] = {nullptr};
	bodyframe->GetAndRefreshBodyData(BODY_COUNT, body);
	for (int i = 0; i < BODY_COUNT; i++) {
		body[i]->get_IsTracked(&tracked);
		if (tracked) {
			body[i]->GetJoints(JointType_Count, joints);
			std::vector<Joint> tempJoints;
			for (auto joint : joints)
			{
				joint.Position.Z = -joint.Position.Z;
				tempJoints.push_back(joint);
			}
			jointsVector.push_back(tempJoints);
			break;
		}
	}
	// ------ END NEW CODE ------

	if (bodyframe) bodyframe->Release();
	return jointsVector;
}

std::vector<std::vector<Joint>> KinectHandler::getBodyJoints()
{
	return m_jointPositions;
}

int KinectHandler::calculateCollision( glm::vec3 start, glm::vec3 direction, IBody* bodies )
{
	int hitBodyIndex = -1;
	float minDistanceFromRay = FLT_MAX;

	direction = glm::normalize( direction );

	for( int currentBodyIndex = 0; currentBodyIndex < BODY_COUNT; ++currentBodyIndex )
	{
		BOOLEAN bTracked = false;
		HRESULT hr = bodies[currentBodyIndex].get_IsTracked( &bTracked );
		if( SUCCEEDED( hr ) && bTracked )
		{
			continue;
		}

		const int jointCount = JointType::JointType_Count;
		Joint joints[jointCount];
		hr = bodies[currentBodyIndex].GetJoints(jointCount, joints);
		if( FAILED( hr ) )
		{
			continue;
		}

		int evaluatedJointCount = 0;
		glm::vec3 averageRayToJoint( 0.0f );
		for( int type = 0; type < jointCount; ++type )
		{
			Joint currentJoint = joints[type];
			if( currentJoint.TrackingState != TrackingState::TrackingState_NotTracked )
			{
				const CameraSpacePoint camSpacePoint = currentJoint.Position;
				const glm::vec3 jointPosition( -camSpacePoint.X, camSpacePoint.Y, -camSpacePoint.Z );
				const glm::vec3 startToJoint = jointPosition - start;
				const float distanceToStart = glm::dot( startToJoint, direction );
				if( distanceToStart < 0 )
				{
					continue;
				}
				const glm::vec3 closestPointOnRay = start + distanceToStart * direction;
				const glm::vec3 rayToJoint = jointPosition - closestPointOnRay;
				averageRayToJoint += rayToJoint;
				++evaluatedJointCount;
			}
		}
		averageRayToJoint /= evaluatedJointCount;
		//use squared length to qvoid squareroot operation
		float distance = glm::length2( averageRayToJoint );

		if( distance < minDistanceFromRay )
		{
			minDistanceFromRay = distance;
			hitBodyIndex = currentBodyIndex;
		}
	}
	return hitBodyIndex;
}

void KinectHandler::retrieveColorPoints(GLfloat* colorData, GLfloat* positionData, HRESULT& hr, int depthWidth, int depthHeight, int colorWidth, int colorHeight)
{
	if (SUCCEEDED(hr))
	{
		colorPoints = new ColorSpacePoint[depthWidth * depthHeight];
		cameraPoints = new CameraSpacePoint[depthWidth * depthHeight];

		// Do the coordinate mapping here
		hr = coordinateMapper->MapDepthFrameToColorSpace(depthWidth * depthHeight, &depthBuffer[0], depthWidth * depthHeight, &colorPoints[0]);
		if (SUCCEEDED(hr))
		{
			hr = coordinateMapper->MapDepthFrameToCameraSpace(depthWidth * depthHeight, &depthBuffer[0], depthWidth * depthHeight, &cameraPoints[0]);

			if (SUCCEEDED(hr))
			{
				fillBuffers(colorData, positionData, depthWidth, depthHeight, colorWidth, colorHeight);
			}//colorspacepoints

		}//cameraspacepoints

		delete[] colorPoints;
		delete[] cameraPoints;
	}
}

void KinectHandler::retrieveCameraIntrinsics() {

	std::cout << "Test retriseve" << std::endl;
	CameraIntrinsics cI = {};

	// waiting for intrinsic parameter propagation
	while (cI.FocalLengthX == 0){
		coordinateMapper->GetDepthCameraIntrinsics(&cI);
	}	
	printCameraIntrinsics(&cI);
}


void KinectHandler::printCameraIntrinsics(CameraIntrinsics* cI){

	std::cout << "Test set and print" << std::endl;

	std::cout << "Value FocLenX: " << cI->FocalLengthX << std::endl;
	std::cout << "Value FocLenY: " << cI->FocalLengthY << std::endl;
	std::cout << "Value PrincPointX: " << cI->PrincipalPointX << std::endl;
	std::cout << "Value PrincPointY: " << cI->PrincipalPointY << std::endl;
	std::cout << "Value Rad Dis Second: " << cI->RadialDistortionSecondOrder << std::endl;
	std::cout << "Value Rad Dis Fourth: " << cI->RadialDistortionFourthOrder << std::endl;
	std::cout << "Value Rad Dis Sixth: " << cI->RadialDistortionSixthOrder << std::endl;
}



void KinectHandler::fillBuffers(GLfloat* colorData, GLfloat* positionData, int depthWidth, int depthHeight, int colorWidth, int colorHeight)
{
	int count = 0;
	// all background points will be drawn on position 1 with color white.
	clearBuffer(positionData, depthWidth * depthHeight * 3);
	clearBuffer(colorData, depthWidth * depthHeight * 3);

	// loop over each row and column of the depth
	for (int y = depthHeight - 1; y >= 0; y--){
		for (int x = 0; x < depthWidth; x++){

			int depthIndex = (y * depthWidth) + x;

			byte body = bodyIndexBuffer[depthIndex];

			// Check whether this pixel belong to a human
			if (body != 0xff){

				CameraSpacePoint cameraPoint = cameraPoints[depthIndex];
				// retrieve the depth to color mapping for the current depth pixel
				ColorSpacePoint colorPoint = colorPoints[depthIndex];

				// make sure the depth pixel maps to a valid point in color space
				int colorX = (int)floor(colorPoint.X + 0.5);
				int colorY = (int)floor(colorPoint.Y + 0.5);

				if ((colorX >= 0) && (colorX < colorWidth) && (colorY >= 0) && (colorY < colorHeight))
				{
					RGBQUAD color = colorBuffer[colorY * colorWidth + colorX];

					colorData[count] = color.rgbRed / 255.0f;
					colorData[count + 1] = color.rgbGreen / 255.0f;
					colorData[count + 2] = color.rgbBlue / 255.0f;

					positionData[count] = cameraPoint.X;
					positionData[count + 1] = cameraPoint.Y;
					positionData[count + 2] = -cameraPoint.Z;
				}
			}
			count += 3;
		}
	}
}


// 
void KinectHandler::clearBuffer(GLfloat *buffer, int size){
	for (int i = 0; i < size; i++)
		buffer[i] = 1.0f; 
}