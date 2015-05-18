#ifndef TERRAIN_H
#define TERRAIN_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath> 
#include "../Modelloader/Model.h"

using namespace std;
using namespace glm;


class Terrain {

public:

Terrain(mat4 modelMatrix, string spKye):positions(0){model = new Model(RESOURCES_PATH "/land.obj", modelMatrix, spKye); } 
~Terrain();

void updatePositions();
vector<float> calculateHeights();
float getHeightCurrentValue(float x, float z);

Model* getModel()const;
vector<vec3> getPositions() const;

void setModel(Model* m);
void setPositions(vector<vec3> pos);


//getters and setters

protected:

 vector<vec3> positions;
 vector<float> hights;
 Model* model;




};



#endif // TERRAIN_H
