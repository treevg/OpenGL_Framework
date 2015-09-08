#include <iostream>
#include <string.h>
#include "glm\glm.hpp"
#include "Leap.h"



class LeapHandler{
public:
	Leap::Controller leapController;
	Leap::HandList handList;
	std::vector<Leap::Bone> boneList;
	bool leftHandPinched = false;

	void LeapHandler::checkForIntersectionGesture();	
	bool LeapHandler::isPinched(Leap::Hand hand);

	std::vector<Leap::Bone> LeapHandler::getBones() const;
	Leap::Hand getRightHand();
	Leap::Hand getLeftHand();
	std::vector<Leap::Vector> getJoints() const;
	std::vector<Leap::Vector> getPalmPositions() const;
	
	bool checkForIntersection(std::vector<glm::vec3> vertices, glm::vec3 O, glm::vec3 D);//, float* out);

	glm::vec3 convertLeapVecToGlm(Leap::Vector leapVector);
	glm::mat4 convertLeapMatToGlm(Leap::Matrix leapMatrix);

private: 
};