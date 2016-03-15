#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/norm.hpp>

namespace CameraObjectRelations
{
	static glm::mat4 getBillboardRotationMatrix(glm::vec3 cameraPosition, glm::vec3 objectCenterPosition)
	{
		//TODO check signing
		glm::vec3 objLookat(0.0f, 0.0f, 1.0f);
		glm::vec3 objToCamera(cameraPosition - objectCenterPosition);
		objToCamera.y = 0;
		if (objToCamera == glm::vec3(0.0f))
		{
			glm::mat4 modelMatrix = glm::mat4(1.0f);
			modelMatrix[0][3] = objectCenterPosition[0];
			modelMatrix[1][3] = objectCenterPosition[1];
			modelMatrix[2][3] = objectCenterPosition[2];
			return modelMatrix;
		}

		objToCamera = glm::normalize(objToCamera);

		glm::vec3 upVec = glm::normalize(glm::cross(objLookat, objToCamera));
		float dot = glm::dot(objToCamera, objLookat);

		glm::mat4 rotation(1.0f);
		if ((dot < 0.9999) && (dot > -0.9999))
		{
			float angle = glm::degrees(glm::acos(dot));
			//float angle = glm::acos(dot);
			rotation = glm::rotate(angle, upVec);
		}
		rotation[3][0] = objectCenterPosition[0];
		rotation[3][1] = objectCenterPosition[1];
		rotation[3][2] = objectCenterPosition[2];

		return rotation;
	}

	static glm::quat rotationBetweenVectors(glm::vec3 start, glm::vec3 dest)
	{
		start = glm::normalize(start);
		dest = glm::normalize(dest);

		float cosTheta = glm::dot(start, dest);
		glm::vec3 rotationAxis;

		if (cosTheta < -1 + 0.001f){
			// special case when vectors in opposite directions:
			// there is no "ideal" rotation axis
			// So guess one; any will do as long as it's perpendicular to start
			rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
			if (glm::length2(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
				rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start);

			rotationAxis = glm::normalize(rotationAxis);
			return glm::angleAxis(180.0f, rotationAxis);
		}

		rotationAxis = glm::cross(start, dest);

		float s = sqrt((1 + cosTheta) * 2);
		float invs = 1 / s;

		return glm::quat(
			s * 0.5f,
			rotationAxis.x * invs,
			rotationAxis.y * invs,
			rotationAxis.z * invs
			);
	}

};