#ifndef COLLISION_OBJECT_H
#define COLLISION_OBJECT_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "ShaderTools/VertexArrayObject.h"
#include "WarppingGame/Player/Player.h"
#include <iostream>
#include <fstream>
#include <cmath> 


using namespace std;
using namespace glm;

struct AABB{
vec3 centerPoint;
float halfWidth;

vec3 min;
vec3 max;

};

enum Mask {
	HAUSE,
	CASTLE , 
	TREE, 
    OTHER};



class CollisionObject {

public:

CollisionObject(VertexArrayObject* vao, mat4, int id, string key);
CollisionObject(VertexArrayObject* vao, mat4, int id);
~CollisionObject();

AABB calculateAABB();
vector<vec3> getUpdatedPositions(vector<float> &xValues, vector<float> &yValues, vector<float> &zValues);
void getMinMax(vec3 &min, vec3 &max);
double squaredDistanceToAABB(const vec3 & p);
bool collidesWothOtherWithPlayer(Player* p);
bool collidesWothOther(CollisionObject &other);
double distanceToCenter(const vec3 & p);


//getters and setters

void setId(int id);
void setVao(VertexArrayObject* vao);
void setModelMatrix(mat4 matrix);
void setAaBb(AABB aabb);

int getId()const;
string getKey() const;
mat4 getModelMatrix() const;
VertexArrayObject* getVao() const;
AABB getAaBb() const;



protected:

 VertexArrayObject* vao;
 mat4 modelMatrix;
 int id;
 vector<vec3> m_boundingBox;
 AABB aabb;
 string key;




};



#endif // COLLISION_OBJECT_H
