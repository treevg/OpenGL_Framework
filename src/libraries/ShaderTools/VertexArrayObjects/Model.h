#ifndef MODEL_H
#define MODEL_H


#include <vector>
#include "../VertexArrayObject.h"
#include "WarppingGame/Modelloader/ObjModelLoader.h"

using namespace std;

class Model : public VertexArrayObject {
public:
	Model(vector<float> positions, vector<float> uvCoordinates);
	Model(const string& path);
	void draw();
	void setnumberOfIndicies(int number);
	
protected:

vector<float> positions;
vector<float> uvCoordinates;
int numberOfIndicies;
ObjModelLoader* loader;

	};

#endif // MODEL_H
