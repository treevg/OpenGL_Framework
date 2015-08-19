#include "LeapMotionHandler.h"

const std::string fingerNames[] = { "Thumb", "Index", "Middle", "Ring", "Pinky" };
const std::string boneNames[] = { "Metacarpal", "Proximal", "Middle", "Distal" };
const std::string stateNames[] = { "STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END" };

vector<Bone> LeapHandler::getBoneList(const Controller& controller) {
	//HandList hands = frame.hands();
	//for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
	//	// Get the first hand
	//	const Hand hand = *hl;
	//	std::string handType = hand.isLeft() ? "Left hand" : "Right hand";
	//	//std::cout << std::string(2, ' ') << handType << ", id: " << hand.id()
	//	//	<< ", palm position: " << hand.palmPosition() << std::endl;
	//	return glm::vec3(hand.palmPosition().x, hand.palmPosition().y, hand.palmPosition().z);
	//}

	//if (!frame.hands().isEmpty()) {
	//	std::cout << std::endl;
	//}
	
	vector<Bone> bones;

	bones.clear();

	// Get the most recent frame and report some basic information
	const Frame frame = controller.frame();
		
	HandList hands = frame.hands();
	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
		// Get the first hand
		const Hand hand = *hl;
		std::string handType = hand.isLeft() ? "Left hand" : "Right hand";
		
		//std::cout << std::string(2, ' ') << handType << ", id: " << hand.id()
		//	<< ", palm position: " << hand.palmPosition() << std::endl;
		
		// Get the hand's normal vector and direction
		const Vector normal = hand.palmNormal();
		const Vector direction = hand.direction();
	
		// Calculate the hand's pitch, roll, and yaw angles
		//std::cout << std::string(2, ' ') << "pitch: " << direction.pitch() * RAD_TO_DEG << " degrees, "
		//	<< "roll: " << normal.roll() * RAD_TO_DEG << " degrees, "
		//	<< "yaw: " << direction.yaw() * RAD_TO_DEG << " degrees" << std::endl;
	
		// Get the Arm bone
		Arm arm = hand.arm();

		// Get fingers
		const FingerList fingers = hand.fingers();

		for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
			const Finger finger = *fl;

			// Get finger bones
			for (int b = 0; b < 4; ++b) {
				Bone::Type boneType = static_cast<Bone::Type>(b);
				Bone bone = finger.bone(boneType);
				
				bones.push_back(bone);
			}
		}
	}

	return bones;
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