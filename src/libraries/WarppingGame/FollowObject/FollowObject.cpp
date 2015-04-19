
#include <cmath>
#include <assert.h>
#include "FollowObject.h"

//TODO Bullet

FollowObject::FollowObject(Camera* camera, float speed) {

   this->m_model = new Model(RESOURCES_PATH "/monkey2.obj", mat4(1), "followObject");
   this->m_speed= speed;
   this->m_currentPosition = vec3(camera->getPosition().x , camera->getPosition().y ,  camera->getPosition().z - 4);
   

}

Model* FollowObject::getSuzanne() const{
return this->m_model;

}

vec3 FollowObject::getCurrentPosition() const{

      return this->m_currentPosition;

}

FollowObject::~FollowObject(){

delete this->m_model;

}


bool FollowObject::moveToTarget(float x, float z, float deltaTime) {

  bool targetReached = false;

//check, if current x and z pos! >70 ! < -70

//position += direction * speed * elapsed;
 vec3 targetZ = vec3(0, this->getCurrentPosition().y, z);
 vec3 targetX = vec3(x, this->getCurrentPosition().y, z);

  cout << "differencre on Z" << abs(targetZ.z -this->m_currentPosition.z) << endl;
  cout << "differencre on X  " << abs(targetX.x - this->m_currentPosition.x) << endl;


  if(abs(targetZ.z - this->m_currentPosition.z) > 0.01 ){

vec3 direction = normalize(targetZ - this->m_currentPosition);

this->m_currentPosition += direction * this->m_speed * deltaTime;

cout << "current position Z " << m_currentPosition.x << " " << m_currentPosition.y << " " << m_currentPosition.z << endl;
  
   targetReached = false;

   //rotate

}else if (abs(targetX.x - this->m_currentPosition.x) > 0.1 ){

  vec3 direction = normalize(targetX - this->m_currentPosition);

 this->m_currentPosition += direction * this->m_speed * deltaTime;

cout << "current position X " << m_currentPosition.x << " " << m_currentPosition.y << " " << m_currentPosition.z << endl;
  
   targetReached = false;

 // cout << "dont compute" << m_currentPosition.x << " " << m_currentPosition.y <<" " << m_currentPosition.z << endl;

  
}else {

    targetReached = true;

}

return targetReached;
  
}


bool FollowObject::rotateX(float angle, float deltaTime){

return false;

}






