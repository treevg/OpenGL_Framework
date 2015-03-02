#ifndef FOLLOW_OBJECT_H
#define FOLLOW_OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include "../Camera/Camera.h"
#include "../../ShaderTools/VertexArrayObjects/Cube.h"

using namespace std;
using namespace glm;


class FollowObject {

public:

FollowObject(Camera* c, float);
~FollowObject();
void moveToTarget(vec3 target,float deltaTime);
void waitForCamera();

Cube* getCube() const;
vec3 getCurrentPosition() const;

//getters and setters

protected:

 vec3 m_currentPosition;
 vec3 m_target;
 vector<vec3> m_boundingBox;
 bool m_waiting;
 float m_speed;
 Cube* m_cube;

};



#endif // FOLLOW_OBJECT_H
