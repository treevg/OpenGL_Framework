#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <vector>
#include <iostream>

using namespace std;
using namespace glm;


enum CameraMode {
   FIRSTPERSON,
   THIRDPERSON,
   LOOKDOWN
};

class Camera {

public:

//default Camera in FirstPersonPerspective  -> to implement 

Camera(CameraMode mode);

Camera();
~Camera();

void update();
void lookAt();
void setCamera(vec3 position, vec3 view, vec3 upVector);
void moveCamera(float speed);
void strafeCamera(float speed);
void rotateAroundObject(float x, float y, float z, vec3 center);
void rotate(float angle, vec3 rotationVector);
void moveWithKeybord();

/* getters and setters */


vec3 getPosition() const;
vec3 getView() const;
vec3 getUpVector() const;
vec3 getStrafeVector() const;
float getSpeed()const;


void setPosition(vec3 position);
void setView(vec3 view);
void setUpVector(vec3 upVector);
void getStrafeVector(vec3 strafeVector);
void setSpeed(float speed);


protected:

vec3 c_position;
vec3 c_view;
vec3 c_upVector;
vec3 c_strafeVector;
float c_speed;
   

};



#endif // CAMERA_H
