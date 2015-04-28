
#include <cmath>
#include <assert.h>
#include "FollowObject.h"

//TODO Bullet

FollowObject::FollowObject(Camera* camera, float speed, mat4 modelMatrix, string sp) {

   this->m_model = new Model(RESOURCES_PATH "/monkey2.obj", modelMatrix, sp);
   this->m_speed= speed;
  // this->m_currentPosition = vec3(camera->getPosition().x , camera->getPosition().y ,  camera->getPosition().z - 4);
    this->m_currentPosition = vec3(123.8 ,-4.5 ,  191.572);

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

vector<vec3> FollowObject::getM_positions() const{

  return this->m_positions;
}



void FollowObject::getPositionsFromFile(string path){

       string line;
       ifstream file (path);
       size_t pos = 0;
       string delimiter = ";";
       std::string xPos;
       std::string zPos;

        if (file.is_open())
  {
      while ( getline (file,line) )
    {

     while ((pos = line.find(delimiter)) != std::string::npos) {
          xPos = line.substr(0, pos);
          zPos=line.erase(0, pos + delimiter.length());
        }
          double xposition = std::stod(xPos);
          double zposition = std::stod(zPos);

     //    cout <<"xPOS " <<  xposition << '\n';
     //    cout <<"zPOS " <<  zposition << '\n';
         vec3 step = vec3(xposition, -4.5, zposition);
         this->m_positions.push_back(step);
       
    }
       cout << "size of m_positions " << this->m_positions.size()<<endl;

  }  else{
       cout << "Unable to open file"; 
  }



}




bool FollowObject::moveToTarget(float x, float z, float deltaTime) {

  bool targetReached = false;

/*

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
*/
return targetReached;
  
}


bool FollowObject::rotateX(float angle, float deltaTime){

return false;

}





vector<VertexArrayObject*> FollowObject::updateModelMatrix(mat4 matrix){
  return this->m_model->updateModelMatrix(matrix);
}
