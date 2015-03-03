#ifndef FOLLOW_OBJECT_H
#define FOLLOW_OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <cmath> 
#include "../Camera/Camera.h"
#include "../Modelloader/Model.h"

using namespace std;
using namespace glm;


class FollowObject {

public:

FollowObject(Camera* c, float);
~FollowObject();
bool moveToTarget(float x, float z,float deltaTime);
bool rotateX(float angle, float deltaTime);
void waitForCamera();

Model* getSuzanne() const;
vec3 getCurrentPosition() const;

//getters and setters

protected:

 vec3 m_currentPosition;
 vec3 m_target;
 vector<vec3> m_boundingBox;
 bool m_waiting;
 float m_speed;
 Model* m_model;




};



#endif // FOLLOW_OBJECT_H
