#ifndef MODEL_H
#define MODEL_H


#include <vector>
#include "../VertexArrayObject.h"

using namespace std;

class Model : public VertexArrayObject {
public:
	Model(vector<float> positions, vector<unsigned int> uvCoordinates);
	void draw();
	
protected:

vector<float> positions;
vector<unsigned int> uvCoordinates;

	};

#endif // MODEL_H
