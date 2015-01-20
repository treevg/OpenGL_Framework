#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <vector>
#include <iostream>

using namespace std;


enum CameraMode {
   FIRSTPERSON,
   THIRDPERSON
};

class Camera {

public:

	Camera(CameraMode mode);
	~Camera();
	void moveCamera(glm::vec3 direction);
	void rotateX(float angle);
	void rotateY(float angle);




protected: 

glm::vec3 position;
glm::vec3 viewOrient;
float fieldOfView; 

};

#endif // CAMERA_H
