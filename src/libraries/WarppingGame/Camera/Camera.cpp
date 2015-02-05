#include "Camera.h"
#include <cmath>

//helpers


//todo: camera by mouse
//implement all functions

/*vec3 c_position;
vec3 c_view;
vec3 c_upVector;
vec3 c_strafeVector; */

/* initial position of camera*/
Camera::Camera(){

this->c_position = vec3(0.0, 0.0, 0.0);
this->c_view = vec3(0.0, 1.0, 0.5);
this->c_upVector = vec3(0.0, 0.0, 1.0); 
this->c_speed = 0.1f;

}
Camera::~Camera(){}

void Camera::update(){}
void Camera::lookAt(){}

void Camera::setCamera(vec3 position, vec3 view, vec3 upVector){

this->c_position = position;
this->c_view = view;
this->c_upVector = upVector; 


}


static float lengthOfVector(vec3 vector){

return (float) sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));

}



static vec3 calculateCrossProd(vec3 vec1, vec3 vec2){

vec3 result;

result.x = ((vec1.y * vec2.z) - (vec1.z * vec2.y));
result.y = ((vec1.z * vec2.x) - (vec1.x * vec2.z));
result.z = ((vec1.x * vec2.y) - (vec1.y * vec2.x));

return result;


}

void Camera::moveCamera(float speed){

vec3 direction = this->c_view - this->c_position;
//normilize direction_vector
direction = direction/lengthOfVector(direction);

//accelration to  new position
this->c_position.x = this->c_position.x + direction.x*this->c_speed;
this->c_position.z = this->c_position.z + direction.z*this->c_speed;
//acceleration to view

this->c_view.x = this->c_view.x + direction.x*this->c_speed;
this->c_view.z = this->c_view.z + direction.z*this->c_speed;


}

/* approch and code for this funktion from http://www.tomdalling.com/blog/modern-opengl/04-cameras-vectors-and-input/ */

void Camera::strafeCamera(float speed){}


void Camera::rotateAroundObject(float x, float y, float z, vec3 center){}


/* approch and code for this funktion from http://www.tomdalling.com/blog/modern-opengl/04-cameras-vectors-and-input/ */

void Camera::rotate(float angle, vec3 rotationVector){

   //direction we are looking

   vec3 direction = c_view - c_position;

   float cosAlpha = (float) cos(angle);
   float sinAlpha = (float) sin (angle);
   vec3 nView;

    nView.x  = (cosAlpha + (1 - cosAlpha) * rotationVector.x * rotationVector.x)	* direction.x;
	nView.x += ((1 - cosAlpha) * rotationVector.x * rotationVector.y - rotationVector.z * sinAlpha)	* direction.y;
	nView.x += ((1 - cosAlpha) * rotationVector.x * rotationVector.z + rotationVector.y * sinAlpha)	* direction.z;
	nView.y  = ((1 - cosAlpha) * rotationVector.x * rotationVector.y + rotationVector.z * sinAlpha)	* direction.x;
	nView.y += (cosAlpha + (1 - cosAlpha) * rotationVector.y * rotationVector.y) * direction.y;
	nView.y += ((1 - cosAlpha) * rotationVector.y * rotationVector.z - rotationVector.x * sinAlpha)	* direction.z;
	nView.z  = ((1 - cosAlpha) * rotationVector.x * rotationVector.z - rotationVector.y * sinAlpha)	* direction.x;
	nView.z += ((1 - cosAlpha) * rotationVector.y * rotationVector.z + rotationVector.x * sinAlpha)	* direction.y;
	nView.z += (cosAlpha + (1 - cosAlpha) * rotationVector.z * rotationVector.z) * direction.z;

	c_view = c_position + nView;

}



void Camera::moveWithKeybord(){}



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

float Camera::getSpeed() const{
return c_speed;

}

void Camera::setPosition(vec3 position){


}
void Camera::setView(vec3 view){

}

void Camera::setUpVector(vec3 upVector){

}


void Camera::getStrafeVector(vec3 strafeVector){

}

void Camera::setSpeed(float speed){
	this->c_speed=speed;

}


/* helpers */




