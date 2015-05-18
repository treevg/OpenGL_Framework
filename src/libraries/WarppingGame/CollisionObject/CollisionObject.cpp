
#include <cmath>
#include <algorithm>
#include <assert.h>
#include "CollisionObject.h"


bool compare(float i, float j) { return i<j ; }

CollisionObject::CollisionObject(VertexArrayObject* vao, mat4 matrix, int id, string key): vao(vao), modelMatrix(matrix), id(id), key(key) {

   this->aabb = calculateAABB();
   cout << "AABB  center point " << this->aabb.centerPoint.x << " " << this->aabb.centerPoint.y << this->aabb.centerPoint.z << endl; 
   cout << "AABB  half width " << this->aabb.halfWidth<< endl; 

}




CollisionObject::~CollisionObject(){
	delete this->vao;
}

AABB CollisionObject::calculateAABB(){
AABB aabb;
vec3 min;
vec3 max;
getMinMax(min, max);
aabb.min = min;
aabb.max = max;

vec3 temp = vec3(min.x, max.y, min.z);
float d = glm::distance(max, temp);
aabb.halfWidth = (float) d/2.0f; 
aabb.centerPoint = vec3((float)(min.x + max.x)/2.f,(float)(min.y + max.y)/2.f, (float)(min.y + max.y)/2.f );

return aabb;
}
vector<vec3> CollisionObject::getUpdatedPositions(vector<float> &xValues, vector<float> &yValues, vector<float> &zValues){
    
  vector<vec3> updated;
 // cout << this->vao << endl;
  cout << this->vao->getVertices().size() << endl;
	for(vec3 vertex: this->vao->getVertices()){

	vec4 temp = this->modelMatrix * vec4(vertex.x,vertex.y,vertex.z, 1);
         updated.push_back(vec3(temp.x, temp.y, temp.z));
         xValues.push_back(temp.x);
         yValues.push_back(temp.y);
         zValues.push_back(temp.z);

	}

	return updated;

}

void CollisionObject::getMinMax(vec3 &min, vec3 &max){
      
      vector<float> xValues;
      vector<float> yValues; 
      vector<float> zValues;


  vector<vec3>  updatedPositions = getUpdatedPositions(xValues, yValues, zValues);
  //  std::sort(xValues.begin(), xValues.end(), compare);
    auto min_itx = std::min_element(xValues.begin(), xValues.end(), compare);
    auto min_ity = std::min_element(yValues.begin(), yValues.end(), compare);
    auto min_itz = std::min_element(zValues.begin(), zValues.end(), compare);

    min.x=*min_itx;
    min.y=*min_ity;
    min.z=*min_itz;

    auto max_itx = std::max_element(xValues.begin(), xValues.end(), compare);
    auto max_ity = std::max_element(yValues.begin(), yValues.end(), compare);
    auto max_itz = std::max_element(zValues.begin(), zValues.end(), compare);

    max.x=*max_itx;
    max.y=*max_ity;
    max.z=*max_itz;

    cout << " MAX " << max.x << "  " <<  max.y << " " <<  max.z << endl;
    cout << " MIN " << min.x << "  " <<  min.y << " " <<  min.z << endl;

	

}


bool CollisionObject::collidesWothOther(CollisionObject &other){
//to implement
  return false;

}	


bool CollisionObject::collidesWothOtherWithPlayer(Player* p){

double squaredDistance = squaredDistanceToAABB(p->getPosition());


  if(this->key == "house1" ){
    
    // cout << "distance to player " << squaredDistance  << endl;
    // cout << " min " <<  this->aabb.min.x  << " " <<this->aabb.min.y  << " " << this->aabb.min.z <<  endl;
    // cout << " max " <<  this->aabb.max.x  << " " <<this->aabb.max.y  << " " << this->aabb.max.z <<  endl;
 
     return squaredDistance+0.01999 <= 0.02 ;

  }else if (this->key == "house2" ){
    
    return squaredDistance <= 0.03;

  }else if (this->key == "tree" ){
    // cout << "distance to tree " << squaredDistance  << endl;   
     return squaredDistance <= 0.02;

  }else if (this->key == "lowpolyhouse" ){
   //  cout << "distance to lowpolyhouse " << squaredDistance  << endl;   
     return squaredDistance+0.01999 <=0.02;

  }else if (this->key == "house3" ){
    cout << "distance to house3 " << squaredDistance  << endl;   
     return squaredDistance+0.01999 <=0.02;

  }
  else{

 //  cout << this->key <<  " distance to player " << squaredDistance << "  rad * rad " << p->getRadius() * p->getRadius() << endl;
   return squaredDistance+0.2 <= (p->getRadius() * p->getRadius());
  
  }
 
  

}

double CollisionObject::squaredDistanceToAABB( const vec3 & p )
{
    auto check = [&]( const double pn,  const double bmin,  const double bmax ) 
    -> double {
        double out = 0;
        double v = pn;
 
        if ( v < bmin )  {  
            double val = (bmin - v);       
                  out += val * val;         }       

          if ( v > bmax )
        {
            double val = double(v - bmax);
            out += val * val;
        }
 
        return out;
    };
 
    // Squared distance
    double sq = 0.000;
 
    sq += check( p.x, this->aabb.min.x, this->aabb.max.x );
    sq += check( p.y, this->aabb.min.y, this->aabb.max.y );
    sq += check( p.z, this->aabb.min.z, this->aabb.max.z );
 
    return sq;
}


double CollisionObject::distanceToCenter( const vec3 & p )
{
    
   double distance = glm::distance(p,this->aabb.centerPoint);
}




/*         getters and setters            */

void CollisionObject::setId(int id){
   this->id=id;
}

void CollisionObject::setVao(VertexArrayObject* vao){
   this->vao=vao;
}

void CollisionObject::setModelMatrix(mat4 matrix){

 this->modelMatrix=matrix;
}

int CollisionObject::getId()const{

 return this->id;
} 
mat4 CollisionObject::getModelMatrix() const{

return this->modelMatrix;
}

VertexArrayObject* CollisionObject::getVao() const{

	return this->vao;

}


AABB CollisionObject::getAaBb() const{

	return this->aabb;

}

void CollisionObject::setAaBb(AABB aabb){
	this->aabb=aabb;
}

string CollisionObject::getKey() const{

   return this->key;
}