#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>

using namespace std;

class ModelLoader {

public:
	
	virtual void loadMesh(const string& path) = 0;
	virtual ~ModelLoader() = default;
	
    

protected: 
	
};


#endif // MODELLOADER_H




