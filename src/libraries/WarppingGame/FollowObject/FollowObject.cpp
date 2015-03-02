
#include <cmath>
#include <assert.h>
#include "FollowObject.h"

//TODO Bullet

FollowObject::FollowObject(Camera* camera, float speed) {
   this->m_cube = new Cube();
   this->m_speed= speed;
   this->m_currentPosition = vec3(camera->getPosition().x , camera->getPosition().y ,  camera->getPosition().z - 4);
   

}

Cube* FollowObject::getCube() const{
return this->m_cube;

}

vec3 FollowObject::getCurrentPosition() const{

      return this->m_currentPosition;

}

FollowObject::~FollowObject(){

delete this->m_cube;

}


void FollowObject::moveToTarget(vec3 target, float deltaTime) {


//check, if current x and z pos! >70 ! < -70

//position += direction * speed * elapsed;
  vec3 direction = normalize(target - this->m_currentPosition);

    this->m_currentPosition += direction * this->m_speed *deltaTime;
  //   while (horizAngle > 360.0f) horizAngle -= 360;
  //  while (this-> > target.x) y_position -= 10;


}







