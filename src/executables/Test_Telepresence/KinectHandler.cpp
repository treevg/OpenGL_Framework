#include "KinectHandler.h"
#include <typeinfo>
#include <vector>

using namespace std;
using namespace glm;


KinectHandler::KinectHandler() :
KinectSensor(NULL),
DepthFrameReader(NULL),
MultiSourceFrameReader(NULL),
coordinateMapper(NULL)
{

}



HRESULT KinectHandler::InitializeDefaultSensor()
{
	HRESULT hr;

	hr = GetDefaultKinectSensor(&KinectSensor);
	if (FAILED(hr))
	{
		return hr;
	}
	cout << "kinectSensor " << KinectSensor << endl;
	if (KinectSensor)
	{
		// Initialize the Kinect and get the depth reader
		//IDepthFrameSource* DepthFrameSource = NULL;

		hr = KinectSensor->Open();

		KinectSensor->get_CoordinateMapper(&coordinateMapper);

		if (SUCCEEDED(hr))
		{
			//hr = KinectSensor->get_DepthFrameSource(&DepthFrameSource);
			// open multiframereader for depth and color
			hr = KinectSensor->OpenMultiSourceFrameReader(FrameSourceTypes_Depth | FrameSourceTypes_Color, &MultiSourceFrameReader);
		}

		//if (SUCCEEDED(hr))
		//{
		//	hr = DepthFrameSource->OpenReader(&DepthFrameReader);
		//}

		//SafeRelease(DepthFrameSource);
	}

	if (!KinectSensor || FAILED(hr))
	{
		//SetStatusMessage(L"No ready Kinect found!", 10000, true);
		cout << "No ready Kinect found!" << endl;
		return E_FAIL;
	}

	return hr;
}


// Main processing function
void KinectHandler::Update(GLfloat *data)
{
	/*if (!DepthFrameReader)
	{
	return;
	}*/

	IMultiSourceFrame* pMultiFrame = NULL;
	IDepthFrame* pDepthFrame = NULL;
	IColorFrame* pColorFrame = NULL;

	IDepthFrameReference* depthReference = NULL;
	IColorFrameReference* colorReference = NULL;

	IFrameDescription* depthFrameDescription = NULL;
	IFrameDescription* colorFrameDescription = NULL;

	int Width = 0;
	int Height = 0;

	int depthWidth = 0;
	int depthHeight = 0;

	int colorWidth = 0;
	int colorHeight = 0;

	//HRESULT hr = DepthFrameReader->AcquireLatestFrame(&pDepthFrame);
	HRESULT hr = MultiSourceFrameReader->AcquireLatestFrame(&pMultiFrame);

	// To Reserve Depth Frame Buffer
	vector<UINT16> depthBuffer;
	// To Reserve Color Frame Buffer
	vector<RGBQUAD> colorBuffer;

	vector<ColorSpacePoint> colorPoints;

	if (SUCCEEDED(hr))
	{

		hr = pMultiFrame->get_DepthFrameReference(&depthReference);
		pDepthFrame = nullptr;

		if (SUCCEEDED(hr))
		{
		
		hr = depthReference->AcquireFrame(&pDepthFrame);


		if (SUCCEEDED(hr))
		{
			hr = pDepthFrame->get_FrameDescription(&depthFrameDescription);

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
				//depthFrameData = new USHORT[depthWidth * depthHeight];
				depthBuffer = vector<UINT16>(depthWidth * depthHeight);
				hr = pDepthFrame->CopyFrameDataToArray(depthBuffer.size(), &depthBuffer[0]);

				if (SUCCEEDED(hr))
				{

					SafeRelease(pDepthFrame);

					hr = pMultiFrame->get_ColorFrameReference(&colorReference);

					if (SUCCEEDED(hr))
					{
						SafeRelease(pMultiFrame);
						pColorFrame = nullptr;
						hr = colorReference->AcquireFrame(&pColorFrame);

						if (SUCCEEDED(hr))
						{
							hr = pColorFrame->get_FrameDescription(&colorFrameDescription);

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
							//colorFrameData = new byte[colorWidth * colorHeight * 4]; //TODO +2??
							colorBuffer = vector<RGBQUAD>(colorWidth * colorHeight);

								if (SUCCEEDED(hr))
								{
									ColorImageFormat format;
									hr = pColorFrame->get_RawColorImageFormat(&format);

									if (SUCCEEDED(hr))
									{
										if (format == ColorImageFormat_Bgra)
											hr = pColorFrame->CopyRawFrameDataToArray(colorBuffer.size() * sizeof(RGBQUAD), reinterpret_cast<BYTE*>(&colorBuffer[0]));
										else
											hr = pColorFrame->CopyConvertedFrameDataToArray(colorBuffer.size() * sizeof(RGBQUAD), reinterpret_cast<BYTE*>(&colorBuffer[0]), ColorImageFormat_Bgra);
									}
									SafeRelease(pColorFrame);

						if (SUCCEEDED(hr))
						{
							colorPoints = vector<ColorSpacePoint>(depthWidth * depthHeight);
							//colorPoints = new ColorSpacePoint[depthWidth * depthHeight];
							hr = coordinateMapper->MapDepthFrameToColorSpace(depthBuffer.size(), &depthBuffer[0], depthBuffer.size(), &colorPoints[0]);
						
							//cout << &depthBuffer[0] << endl; // TODO: wegen ...[0] kackt er ab! oO WAAARUM, KMENG? Alles wird gut Freunde, ich habs gefixt!


							if (SUCCEEDED(hr))
							{
								int count = 0;

								// loop over each row and column of the depth
								for (int y = depthHeight - 1; y >= 0; y--){
									for (int x = 0; x < depthWidth; x++){

										/// AB HIER FUKNTIONIERT AUCH, NUR WOLLEN WIR HIER MIT MapDepthFrameToColorSpace ARBEITEN und tun es jetzt auch, bin mir aber unsicher ob es wirklich besser aussieht
										// calculate index into depth array
										int depthIndex = (y * depthWidth) + x;

										// retrieve the depth to color mapping for the current depth pixel
										ColorSpacePoint colorPoint = colorPoints[depthIndex];


										// make sure the depth pixel maps to a valid point in color space
										int colorX = (int)floor(colorPoint.X + 0.5);
										int colorY = (int)floor(colorPoint.Y + 0.5);

										if ((colorX >= 0) && (colorX < colorWidth) && (colorY >= 0) && (colorY < colorHeight))
										{
											// calculate index into color array
											int colorIndex = ((colorY * colorWidth) + colorX) * 4;

											RGBQUAD color = colorBuffer[colorY * colorWidth + colorX];

											data[count] = color.rgbRed / 255.0f;
											data[count + 1] = color.rgbGreen / 255.0f;
											data[count + 2] = color.rgbBlue / 255.0f;
										}
										count += 3;



										///// AB HIER IST ALTER FUNKTIONIERENDER CODE, ABER RUCKELT, DA MapDepthPointToColorSpace

										//DepthSpacePoint depthSpacePoint = { static_cast<float>(x), static_cast<float>(y) };
										//UINT16 depth = depthBuffer[y * depthWidth + x];
										//ColorSpacePoint colorSpacePoint = { 0.0f, 0.0f };

										//coordinateMapper->MapDepthPointToColorSpace(depthSpacePoint, depth, &colorSpacePoint);
										//int colorX = static_cast<int>(std::floor(colorSpacePoint.X + 0.5f));
										//int colorY = static_cast<int>(std::floor(colorSpacePoint.Y + 0.5f));

										//if ((0 <= colorX) && (colorX < colorWidth) && (0 <= colorY) && (colorY < colorHeight)){
										//	RGBQUAD color = colorBuffer[colorY * colorWidth + colorX];

										//	data[count] = color.rgbRed / 255.0f;
										//	data[count + 1] = color.rgbGreen / 255.0f;
										//	data[count + 2] = color.rgbBlue / 255.0f;
										//}
										//count += 3;
									}
								}
								}//colorspacepoints
							}//converted color
							}//colorbuffer
						 }//framedescription color width u height
					}//colorframe
				}//colorreference
				}//depthbuffer
				} // framedescription depth width u height
		} //dephtframe
		} //dephtframereference







		//INT64 Time = 0;

		//USHORT DepthMinReliableDistance = 0;
		//USHORT DepthMaxDistance = 0;
		//UINT BufferSize = 0;
		//UINT16 *Buffer = NULL;

		//hr = pDepthFrame->get_RelativeTime(&Time);

		//if (SUCCEEDED(hr))
		//{
		//	hr = pDepthFrame->get_FrameDescription(&FrameDescription);
		//}

		//if (SUCCEEDED(hr))
		//{
		//	hr = FrameDescription->get_Width(&Width);
		//}

		//if (SUCCEEDED(hr))
		//{
		//	hr = FrameDescription->get_Height(&Height);
		//}

		//if (SUCCEEDED(hr))
		//{
		//	hr = pDepthFrame->get_DepthMinReliableDistance(&DepthMinReliableDistance);
		//}

		//if (SUCCEEDED(hr))
		//{
		//	// In order to see the full range of depth (including the less reliable far field depth)
		//	// we are setting nDepthMaxDistance to the extreme potential depth threshold
		//	DepthMaxDistance = USHRT_MAX;

		//	// Note:  If you wish to filter by reliable depth distance, uncomment the following line.
		//	//// hr = pDepthFrame->get_DepthMaxReliableDistance(&nDepthMaxDistance);
		//}

		//if (SUCCEEDED(hr))
		//{
		//	hr = pDepthFrame->AccessUnderlyingBuffer(&BufferSize, &Buffer);
		//}

		//if (SUCCEEDED(hr))
		//{
		//	ProcessDepth(Time, Buffer, Width, Height, DepthMinReliableDistance, DepthMaxDistance, data);
		//}

		SafeRelease(colorFrameDescription);
		SafeRelease(depthFrameDescription);

	}  // multiframe
}

/// <summary>
/// Handle new depth data
/// <param name="nTime">timestamp of frame</param>
/// <param name="pBuffer">pointer to frame data</param>
/// <param name="nWidth">width (in pixels) of input image data</param>
/// <param name="nHeight">height (in pixels) of input image data</param>
/// <param name="nMinDepth">minimum reliable depth</param>
/// <param name="nMaxDepth">maximum reliable depth</param>
/// </summary>
void KinectHandler::ProcessDepth(INT64 Time, const UINT16* Buffer, int Width, int Height, USHORT MinDepth, USHORT MaxDepth, GLfloat *data)
{
	//if (m_hWnd)
	//{
	//	if (!m_nStartTime)
	//	{
	//		m_nStartTime = nTime;
	//	}

	//	double fps = 0.0;

	//	LARGE_INTEGER qpcNow = { 0 };
	//	if (m_fFreq)
	//	{
	//		if (QueryPerformanceCounter(&qpcNow))
	//		{
	//			if (m_nLastCounter)
	//			{
	//				m_nFramesSinceUpdate++;
	//				fps = m_fFreq * m_nFramesSinceUpdate / double(qpcNow.QuadPart - m_nLastCounter);
	//			}
	//		}
	//	}

	//	WCHAR szStatusMessage[64];
	//	StringCchPrintf(szStatusMessage, _countof(szStatusMessage), L" FPS = %0.2f    Time = %I64d", fps, (nTime - m_nStartTime));

	//	if (SetStatusMessage(szStatusMessage, 1000, false))
	//	{
	//		m_nLastCounter = qpcNow.QuadPart;
	//		m_nFramesSinceUpdate = 0;
	//	}
	//}

	// Make sure we've received valid data
	if (Buffer && (Width == DepthWidth) && (Height == DepthHeight))
	{
		//pRGBX = m_pDepthRGBX;
		//vec4 color = vec4(0.0f, 0.0f, 0.0f, 0.0f);

		// end pixel is start + width*height - 1
		const UINT16* BufferEnd = Buffer + (Width * Height);

		int count = 0;
		for (Buffer; Buffer < BufferEnd; Buffer++)
		{
			USHORT depth = *Buffer;
			float intensity = static_cast<float>((depth >= MinDepth) && (depth <= MaxDepth) ? (depth % 256) : 0);

			data[count] = intensity / 255.0;
			data[count + 1] = intensity / 255.0;
			data[count + 2] = intensity / 255.0;

			count += 3;
		}

		//while (Buffer < BufferEnd)
		//{
		//	USHORT depth = *Buffer;

		//	// To convert to a byte, we're discarding the most-significant
		//	// rather than least-significant bits.
		//	// We're preserving detail, although the intensity will "wrap."
		//	// Values outside the reliable depth range are mapped to 0 (black).

		//	// Note: Using conditionals in this loop could degrade performance.
		//	// Consider using a lookup table instead when writing production code.
		//	BYTE intensity = static_cast<BYTE>((depth >= MinDepth) && (depth <= MaxDepth) ? (depth % 256) : 0);

		//	for (int i = 0; i < 3; ++i)
		//		*data++ = rand() / RAND_MAX;
		//		//*data++ = (BYTE)depth % 256;

		//
		//
		//	

		//	//color.r = intensity;
		//	//color.g = intensity;
		//	//color.b = intensity;

		//	//pRGBX->rgbRed = intensity;
		//	//pRGBX->rgbGreen = intensity;
		//	//pRGBX->rgbBlue = intensity;

		//	

		//	//++pRGBX;
		//	++Buffer;
		//}

		// Draw the data with Direct2D
		//m_pDrawDepth->Draw(reinterpret_cast<BYTE*>(m_pDepthRGBX), cDepthWidth * cDepthHeight * sizeof(RGBQUAD));

	}
}