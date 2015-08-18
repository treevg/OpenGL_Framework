#include <iostream>
#include <string.h>
#include "glm\glm.hpp"
#include "Leap.h"

using namespace Leap;
using namespace std;

class LeapMotionHandler{
public:
	Controller leapController;
	HandList handList;
	Hand leftHand;
	Hand rightHand;
	vector<Bone> boneList;
	bool leftHandPinched = false;

	void LeapMotionHandler::updateLeap();
	void LeapMotionHandler::checkForIntersectionGesture();	
	void LeapMotionHandler::detectGestures();
	vector<Bone> LeapMotionHandler::getBoneList();
	bool checkForIntersection(std::vector<glm::vec3> vertices, glm::vec3 O, glm::vec3 D);//, float* out);


	glm::vec3 convertLeapVecToGlm(Leap::Vector leapVector);
	glm::mat4 convertLeapMatToGlm(Leap::Matrix leapMatrix);
	
private: 
};