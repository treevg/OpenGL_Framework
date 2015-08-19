#include "KinectHandler.h"
#include <typeinfo>
#include "stdafx.h"

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
coordinateMapper(NULL),
colorPoints(NULL),
cameraPoints(NULL),
depthBuffer(NULL),
colorBuffer(NULL),
bodyIndexBuffer(NULL)
{
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

		if (SUCCEEDED(hr))
		{
			// open multiframereader for depth and color
			hr = kinectSensor->OpenMultiSourceFrameReader(FrameSourceTypes_Depth | FrameSourceTypes_Color | FrameSourceTypes_BodyIndex, &multiSourceFrameReader);
		}
	}

	if (!kinectSensor || FAILED(hr))
	{
		//SetStatusMessage(L"No ready Kinect found!", 10000, true);
		cout << "No ready Kinect found!" << endl;
		return E_FAIL;
	}

	return hr;
}



// Main processing function
void KinectHandler::updateKinect(GLfloat *data)
{
	IMultiSourceFrame* pMultiFrame = NULL;
	HRESULT hr = multiSourceFrameReader->AcquireLatestFrame(&pMultiFrame);

	if (SUCCEEDED(hr))
	{
		int Width = 0;
		int Height = 0;
		int depthWidth = 0;
		int depthHeight = 0;
		int colorWidth = 0;
		int colorHeight = 0;

		hr = pMultiFrame->get_DepthFrameReference(&depthReference);
		depthFrame = nullptr;

		if (SUCCEEDED(hr))
		{
		
		hr = depthReference->AcquireFrame(&depthFrame);


		if (SUCCEEDED(hr))
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

			if (SUCCEEDED(hr))
			{
				depthBuffer = new UINT16[depthWidth * depthHeight];
				hr = depthFrame->CopyFrameDataToArray(depthWidth * depthHeight, &depthBuffer[0]);
				//depthBuffer = vector<UINT16>(depthWidth * depthHeight);
				//hr = pDepthFrame->CopyFrameDataToArray(depthBuffer.size(), &depthBuffer[0]);

				if (SUCCEEDED(hr))
				{

					SafeRelease(depthFrame);

					hr = pMultiFrame->get_ColorFrameReference(&colorReference);

					if (SUCCEEDED(hr))
					{
						SafeRelease(pMultiFrame);
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
													 

						 if (SUCCEEDED(hr))
						 {
							colorBuffer = new RGBQUAD[colorWidth * colorHeight];
							//colorBuffer = vector<RGBQUAD>(colorWidth * colorHeight);

								if (SUCCEEDED(hr))
								{
									ColorImageFormat format;
									hr = colorFrame->get_RawColorImageFormat(&format);

									if (SUCCEEDED(hr))
									{
										if (format == ColorImageFormat_Bgra)
											hr = colorFrame->CopyRawFrameDataToArray(colorWidth * colorHeight * sizeof(RGBQUAD), reinterpret_cast<BYTE*>(&colorBuffer[0]));
										else
											hr = colorFrame->CopyConvertedFrameDataToArray(colorWidth * colorHeight * sizeof(RGBQUAD), reinterpret_cast<BYTE*>(&colorBuffer[0]), ColorImageFormat_Bgra);
									}
									SafeRelease(colorFrame);

						if (SUCCEEDED(hr))
						{
							//colorPoints = vector<ColorSpacePoint>(depthWidth * depthHeight);
							colorPoints = new ColorSpacePoint[depthWidth * depthHeight];
							hr = coordinateMapper->MapDepthFrameToColorSpace(depthWidth * depthHeight, &depthBuffer[0], depthWidth * depthHeight, &colorPoints[0]);
						
							if (SUCCEEDED(hr))
							{
								int count = 0;

								// loop over each row and column of the depth
								for (int y = depthHeight - 1; y >= 0; y--){
									for (int x = 0; x < depthWidth; x++){

										// calculate index into depth array
										int depthIndex = (y * depthWidth) + x;

										// retrieve the depth to color mapping for the current depth pixel
										ColorSpacePoint colorPoint = colorPoints[depthIndex];


										// make sure the depth pixel maps to a valid point in color space
										int colorX = (int)floor(colorPoint.X + 0.5);
										int colorY = (int)floor(colorPoint.Y + 0.5);

										if ((colorX >= 0) && (colorX < colorWidth) && (colorY >= 0) && (colorY < colorHeight))
										{
											RGBQUAD color = colorBuffer[colorY * colorWidth + colorX];

											data[count]		= color.rgbRed / 255.0f;
											data[count + 1] = color.rgbGreen / 255.0f;
											data[count + 2] = color.rgbBlue / 255.0f;
										}
										count += 3;
									}
								}
								}//colorspacepoints
							delete[] colorPoints;
							}//converted color
							}//colorbuffer

							delete[] colorBuffer;
						 }//framedescription color width u height
					}//colorframe
				}//colorreference
				}//depthbuffer
				
			delete[] depthBuffer;
			} // framedescription depth width u height
		} //dephtframe
		} //dephtframereference

		SafeRelease(colorFrameDescription);
		SafeRelease(depthFrameDescription);

	}  // multiframe
}



// Main processing function
void KinectHandler::updateKinect(GLfloat* colorData, GLfloat* positionData)
{
	//m_colorData = new float[depthWidth * depthHeight * 3];
	//m_positionData = new float[depthWidth * depthHeight * 3];

	IMultiSourceFrame* pMultiFrame = NULL;
	HRESULT hr = multiSourceFrameReader->AcquireLatestFrame(&pMultiFrame);

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
		depthFrame = nullptr;

		if (SUCCEEDED(hr))
		{
			hr = depthReference->AcquireFrame(&depthFrame);

			if (SUCCEEDED(hr))
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

				if (SUCCEEDED(hr))
				{
					depthBuffer = new UINT16[depthWidth * depthHeight];

					// Update the depth data
					hr = depthFrame->CopyFrameDataToArray(depthWidth * depthHeight, &depthBuffer[0]);
			
					if (SUCCEEDED(hr))
					{
						hr = depthFrame->get_DepthMinReliableDistance(&minReliableDistance);
					}
						
					if (SUCCEEDED(hr))
					{
						hr = depthFrame->get_DepthMaxReliableDistance(&maxReliableDistance);
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

										SafeRelease(pMultiFrame);

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

											SafeRelease(bodyIndexFrame);
											
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
														int count = 0;
														clearBuffer(positionData, depthWidth * depthHeight * 3);
														clearBuffer(colorData, depthWidth * depthHeight * 3);

														// loop over each row and column of the depth
														for (int y = depthHeight - 1; y >= 0; y--){
															for (int x = 0; x < depthWidth; x++){

																// calculate index into depth array
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
													
																		//if (cameraPoint.X != -INFINITY && cameraPoint.Y != -INFINITY && cameraPoint.Z != -INFINITY)
																		//{
																			//depthData[count] = (float)x / (float)depthWidth;
																			//depthData[count + 1] = (float)(-y) / (float)depthHeight;
																			//depthData[count + 2] = -(float)(depthBuffer[depthIndex] - minReliableDistance) / (float)(maxReliableDistance - minReliableDistance);

																		positionData[count] = -cameraPoint.X;
																		positionData[count + 1] = cameraPoint.Y;
																		positionData[count + 2] = -cameraPoint.Z;

																		//}
																	}
																}
																count += 3;
															}
														}
													}//colorspacepoints
												}//cameraspacepoints
												delete[] colorPoints;
												delete[] cameraPoints;
											}//converted color
											delete[] bodyIndexBuffer;
										}//bodyindexbuffer
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
}


void KinectHandler::clearBuffer(GLfloat *buffer, int size){
	for (int i = 0; i < size; i++)
		buffer[i] = 1.0f; 
}