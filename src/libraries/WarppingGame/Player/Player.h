#ifndef PLAYER_H
#define PLAYER_H

#define GLM_FORCE_RADIANS 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

using namespace std;
using namespace glm;



class Player {

public:

Player(vec3 position, float radius);
Player(): radius(3.0f){}
~Player();


vec3 getPosition() const;
vec3 getView() const;
float getRadius() const;
void setPosition(vec3 position);
void setView(vec3 view);
void setRadius(float radius);


protected:

glm::vec3 position;
glm::vec3 view;
float radius;
   

};



#endif // CAMERA_H
