#include <iostream>
#include <string.h>
#include "glm\glm.hpp"
#include "Leap.h"



class LeapHandler{
public:
	Leap::Controller leapController;
	Leap::HandList handList;
	Leap::Hand leftHand;
	Leap::Hand rightHand;
	std::vector<Leap::Bone> boneList;
	bool leftHandPinched = false;

	void LeapHandler::updateLeap();
	void LeapHandler::checkForIntersectionGesture();	
	void LeapHandler::detectGestures();
	std::vector<Leap::Bone> LeapHandler::getBones() const;
	bool checkForIntersection(std::vector<glm::vec3> vertices, glm::vec3 O, glm::vec3 D);//, float* out);


	glm::vec3 convertLeapVecToGlm(Leap::Vector leapVector);
	glm::mat4 convertLeapMatToGlm(Leap::Matrix leapMatrix);

	std::vector< Leap::Vector > getJoints() const;
	std::vector<Leap::Vector> getPalmPositions() const;
	
private: 
};