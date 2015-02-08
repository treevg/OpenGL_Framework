#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
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

mat4 looksAt();
void setCamera(vec3 position, vec3 view, vec3 upVector);
void moveCamera(float speed);
void strafeCamera(float speed);
void rotatePoint(float x, float y, float z, vec3 center);
void rotate(float angle, vec3 rotationVector);
void rotateWithMouse();

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

glm::vec3 c_position;
glm::vec3 c_view;
glm::vec3 c_upVector;
glm::vec3 c_strafeVector;
float c_speed;
   

};



#endif // CAMERA_H
