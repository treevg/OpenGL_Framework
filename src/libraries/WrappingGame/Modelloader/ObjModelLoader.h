#ifndef OBJMODELLOADER_H
#define OBJMODELLOADER_H

#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>
#include "ModelLoader.h"

/*

this class represents a Loader for .obj format
obj contains

  # List of Vertices, with (x,y,z[,w]) coordinates
  v 0.123 0.234 0.345 1.0
  
  # Texture coordinates, in (u, v [,w]) coordinates, these will vary between 0 and 1, w is optional and defaults to 0.
  vt 0.500 1 [0]
  vt ...
  ...
  # Normals in (x,y,z) form; normals might not be unit.
  vn 0.707 0.000 0.707
  vn ...
  ...
  # Parameter space vertices in ( u [,v] [,w] ) form; free form geometry statement ( see below )
  vp 0.310000 3.210000 2.100000
  vp ...
  ...
  # Face Definitions (see below)
  f 1 2 3
  f 3/1 4/2 5/3
  f 6/4/1 3/5/3 7/6/5
  f ...
  ... 
  additionally 
*/

using namespace std;


struct Face {         /* deklariert den Strukturtyp person */
    int   vertexPos;
    int   texturePos;
    int   normalPos;
  };

class ObjModelLoader: public ModelLoader {

public:
	
     ObjModelLoader(const string& path);
     void loadMesh(const string& path);


protected: 
    vector<float>positions;
    vector<unsigned int> texturePositions;
    vector<float> normals;
    vector<Face> faces; 


	
};


#endif // OBJMODELLOADER_H
