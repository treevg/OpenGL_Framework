#include "Camera.h"

//helpers


//todo: camera by mouse
//implement all functions

/*vec3 c_position;
vec3 c_view;
vec3 c_upVector;
vec3 c_strafeVector; */


Camera::Camera(){

}
Camera::~Camera(){}

void Camera::update(){}
void Camera::lookAt(){}
void Camera::setCamera(vec3 position, vec3 view, vec3 upVector){}
void Camera::moveCamera(float speed){}
void Camera::strafeCamera(float speed){}
void Camera::rotateAroundObject(float x, float y, float z, vec3 center){}
void Camera::rotate(float angle, vec3 rotationVector){}




/* getters and setters */


vec3 Camera::getPosition() const{
	return this->c_position;
}
vec3 Camera::getView() const{

	return this->c_view;
}
vec3 Camera::getUpVector() const{
return this->c_upVector;

}
vec3 Camera::getStrafeVector() const{


}

void Camera::setPosition(vec3 position){


}
void Camera::setView(vec3 view){

}
void Camera::setUpVector(vec3 upVector){

}
void Camera::getStrafeVector(vec3 strafeVector){


}




