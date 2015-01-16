#include <stdexcept>
#include <fstream>

#include "ObjModelLoader.h"



/*   vector<float>positions;
    vector<unsigned int> texturePositions;
    vector<float> normals;
    vector<Face> faces;  

struct Face {         
    int   vertexPos;
    int   texturePos;
    int   normalPos;
  };
    */


ObjModelLoader::ObjModelLoader(const string& path){

	try{
          this->loadMesh(path);

	}catch(std::exception ex){

          std::cerr << "Exception :" << ex.what() << '\n';

	}

	
}


void ObjModelLoader::loadMesh(const string& path) {
    
    vector<string> list; 

    ifstream reader; 

	reader.open(path, ios::in);
	
	if(!reader) {

     throw std::exception();

	}else {
   

   			string line; 
      
			while(!reader.eof()){

				reader >> line;
				list.push_back(line);

			}

     for (int i = 0; i < list.size(); ++i)
     {
     	cout<<list[i] << endl;
     }

	}


}




