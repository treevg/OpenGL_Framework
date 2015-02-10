#include "Model.h"

using namespace Assimp;

 Model::Model(const string& path){
 //implement
 }

 Model::~Model(){
 	
 }
 

 vector<Mesh>  Model::getMeshes() const{

     	return this->meshes;
     }

    
   bool  Model::loadModel(const string& path){
     
      bool loaded = false;

      Importer importer;
      const aiScene* scene = importer.ReadFile( path,  aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |aiProcess_FlipUVs);
      
      //check for errors
     if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {

     	cout << "Could not load model. ERROR: " << importer.GetErrorString() << endl;

         loaded = false;

     }

     //IMPLEMENT


     return loaded;


    }


   void  Model::processNode(aiNode* node, const aiScene* scene){


    }

    Mesh  Model::convertMesh(const aiScene* scene, aiMesh* mesh){ }