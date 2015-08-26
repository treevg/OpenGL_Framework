#include "LeapHandler.h"

const std::string fingerNames[] = { "Thumb", "Index", "Middle", "Ring", "Pinky" };
const std::string boneNames[] = { "Metacarpal", "Proximal", "Middle", "Distal" };
const std::string stateNames[] = { "STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END" };

using namespace Leap;

void LeapHandler::updateLeap(){

	const Frame frame = leapController.frame();

	handList = frame.hands();
	for (HandList::const_iterator h = handList.begin(); h != handList.end(); ++h){
		const Hand hand = *h;
		if (hand.isLeft()){
			leftHand = hand;
		}
		else{
			rightHand = hand;
		}

	}
	detectGestures();
}

std::vector<Bone> LeapHandler::getBones() const
{	
	std::vector<Leap::Bone> boneList;

	for (auto hand : leapController.frame().hands())
	{
		// Get fingers
		const FingerList fingers = hand.fingers();

		for ( auto finger : fingers)
		{
			// Get finger bones
			for (int b = 0; b < 4; ++b) {
				Bone::Type boneType = static_cast<Bone::Type>(b);
				Bone bone = finger.bone(boneType);
				
				boneList.push_back(bone);
			}
		}
	}

	return boneList;
}

void LeapHandler::detectGestures(){
	if (leftHand.pinchStrength() >= 0.7)
		leftHandPinched = true;
	else leftHandPinched = false;
}

bool LeapHandler::checkForIntersection(std::vector<glm::vec3> vertices, glm::vec3 O, glm::vec3 D){
	glm::vec3 V1, V2, V3; 

	glm::vec3 e1, e2;  //Edge1, Edge2
	glm::vec3 P, Q, T;
	float det, inv_det, u, v;
	float t;

	bool intersection = false;

	for (int vertex = 0; vertex < vertices.size(); vertex += 3)
	{
		V1 = vertices.at(vertex);
		V2 = vertices.at(vertex + 1);
		V3 = vertices.at(vertex + 2);


		//Find vectors for two edges sharing V1
		e1 = V2 - V1;
		e2 = V3 - V1;
		//Begin calculating determinant - also used to calculate u parameter
		P = glm::cross(D, e2);
		//if determinant is near zero, ray lies in plane of triangle
		det = glm::dot(e1, P);
		//NOT CULLING
		if (det > -0.000001 && det < 0.000001)
			continue;
		inv_det = 1.f / det;

		//calculate distance from V1 to ray origin
		T = O - V1;

		//Calculate u parameter and test bound
		u = glm::dot(T, P) * inv_det;
		//The intersection lies outside of the triangle
		if (u < 0.f || u > 1.f) 
			continue;

		//Prepare to test v parameter
		Q = glm::cross(T, e1);

		//Calculate V parameter and test bound
		v = glm::dot(D, Q) * inv_det;
		//The intersection lies outside of the triangle
		if (v < 0.f || u + v  > 1.f) 
			continue;

		t = glm::dot(e2, Q) * inv_det;

		if (t > 0.000001) { //ray intersection
			//*out = t;
			//cout << "intersection at: " << t << endl;
			intersection = true;
		}
	}

	return intersection;
}

glm::vec3 LeapHandler::convertLeapVecToGlm(Leap::Vector leapVector){
	return glm::vec3(leapVector.x, leapVector.y, leapVector.z);
}

glm::mat4 LeapHandler::convertLeapMatToGlm(Leap::Matrix leapMatrix){
	return	glm::mat4(
		glm::vec4(leapMatrix.xBasis.x, leapMatrix.xBasis.y, leapMatrix.xBasis.z, 0),
		glm::vec4(leapMatrix.yBasis.x, leapMatrix.yBasis.y, leapMatrix.yBasis.z, 0),
		glm::vec4(leapMatrix.zBasis.x, leapMatrix.zBasis.y, leapMatrix.zBasis.z, 0),
		glm::vec4(0, 0, 0, 1));
}

std::vector<Leap::Vector> LeapHandler::getJoints() const
{
	std::vector< Leap::Vector> joints;
	std::vector< Leap::Bone > bones = getBones();
	for ( auto bone : bones )
	{
		if (bone.type() == Leap::Bone::TYPE_METACARPAL)
		{
			joints.push_back(bone.prevJoint());
		}
		joints.push_back( bone.nextJoint() );
	}
	return joints;
}

std::vector<Leap::Vector> LeapHandler::getPalmPositions() const
{
	std::vector<Leap::Vector> palmPositions;
	for (auto hand : leapController.frame().hands())
	{
		palmPositions.push_back(hand.palmPosition());
	}
	return palmPositions;
}