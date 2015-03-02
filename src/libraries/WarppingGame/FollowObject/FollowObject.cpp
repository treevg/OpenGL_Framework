
#include <cmath>
#include <assert.h>
#include "FollowObject.h"

//TODO Bullet

FollowObject::FollowObject(Camera* camera){
   this->m_cube = new Cube();
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


void FollowObject::moveToTarget(vec3 target) {

//move to center of the castle  and calculate left or right side




}







