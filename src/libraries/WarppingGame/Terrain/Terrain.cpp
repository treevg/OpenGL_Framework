
#include <cmath>
#include <assert.h>
#include "Terrain.h"


void Terrain::updatePositions(){

	vector<VertexArrayObject*> terrainMeshes = this->model->getMeshes();
	mat4 modelMatrix = this->model->getModelMatrix();
	
	for(VertexArrayObject* vao: terrainMeshes){

	for(vec3 vertex: vao->getVertices()){

	 vec4 temp = modelMatrix * vec4(vertex.x,vertex.y,vertex.z, 1);
         this->positions.push_back(vec3(temp.x, temp.y, temp.z));

         }

}

}

 Terrain::~Terrain(){
     delete model;
}


vector<float> Terrain::calculateHeights(){
       vector<float> heights;
   for(vec3 vertex: this->positions){
	
	 float temp = vertex.z;
         heights.push_back(temp);
         }

return heights;

}
float Terrain::getHeightCurrentValue(float x, float z){

return 1.0f;

}

Model* Terrain::getModel() const{

	return this->model;

}

vector<vec3> Terrain::getPositions() const{

	return this->positions;

}

void Terrain::setModel(Model* m){

	this->model=m;

}

void Terrain::setPositions(vector<vec3> pos){

	this->positions=pos;
}




