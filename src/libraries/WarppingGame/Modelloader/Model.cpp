#include "Model.h"

using namespace Assimp;

 Model::Model(const string& path){
 bool loaded = loadModel(path);
    assert (loaded); 

 cout << "status " << loaded << endl;
 }

 Model::~Model(){
	this->meshes.clear();
 }
 

 vector<Mesh*>  Model::getMeshes() const{

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

     }else {

      cout << "Number of  meshes" <<  scene->mNumMeshes << endl;
      cout << "Number of  materials" <<  scene->mNumMaterials << endl;
      cout << "Number of  textures" <<  scene->mNumTextures << endl;

     cout << "Model loaded from : " << path << endl;
     
     this->processNode(scene->mRootNode, scene);

     loaded = true;

     }
 

     return loaded;


    }


   static  vector<GLuint>  getIndexFromFace(aiMesh* aSmesh){
        vector<GLuint> indices;
        indices.clear();

        cout << "Number of  faces" <<  aSmesh->mNumFaces << endl;

        for(int i = 0; i < aSmesh->mNumFaces; i++)
        {
            aiFace face = aSmesh->mFaces[i];
        
          
            for(int j = 0; j < face.mNumIndices; j++)

                indices.push_back(face.mIndices[j]);
        }
      
        return indices;

    }



/*  static  vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName,vector<Texture> textures_loaded)
    {
        vector<Texture> textures;

        for(GLuint i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            
            GLboolean skip = false;
            for(GLuint j = 0; j < this->textures_loaded.size(); j++)
            {
                if(textures_loaded[j].path == str)
                {
                    textures.push_back(this->textures_loaded[j]);
                    skip = true; 
                    break;
                }
            }
            if(!skip)
            {  
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = str;
                textures.push_back(texture);
                this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
        }
        return textures;
    }
}*/

    static void getMaterialFromMesh(const aiScene* scene, aiMesh* aSmesh){

         if(scene->HasMaterials())
        {

            cout << "number of materials " << scene->mNumMaterials << endl;
            aiMaterial* material = scene->mMaterials[aSmesh->mMaterialIndex];

        }


    }


    //for every mesh of the model

    Mesh* Model::convertMesh(const aiScene* scene, aiMesh* aSmesh){

       
    vector<vec3> vertices;
    vector<vec3> normals;
    vector<vec2> texCoords;
    vector<GLuint> indices;
    vector<Texture> textures;

       for(int i = 0; i < aSmesh->mNumVertices; i++)
        {      
         if (aSmesh->HasPositions()) {
                    vec3 position;
                    position.x = aSmesh->mVertices[i].x;
                    position.y = aSmesh->mVertices[i].y;
                    position.z = aSmesh->mVertices[i].z;
                    vertices.push_back(position);

                  }

                if(aSmesh->HasNormals()){

                   vec3 normal;

                    normal.x = aSmesh->mNormals[i].x;
                    normal.y = aSmesh->mNormals[i].y;
                    normal.z = aSmesh->mNormals[i].z;
                   
                    normals.push_back(normal);

                } 


           if (aSmesh->HasTextureCoords(0)) {
       //     cout << "extracting texture coordinates" << endl;
                vec2 textCoord;

                textCoord.x = aSmesh->mTextureCoords[0][i].x; 
                textCoord.y = aSmesh->mTextureCoords[0][i].y;
                texCoords.push_back(textCoord);
                
            }
            else{

              vec2  textCoord2 = vec2(0.0f, 0.0f);
              texCoords.push_back(textCoord2);

            }
         
        }

           indices = getIndexFromFace(aSmesh);

           //   cout << "size of indices after filling " << indices.size() << endl;


           getMaterialFromMesh(scene, aSmesh);

        //  cout << "size of texture coordinates " << texCoords.size() << endl;

     Mesh* m = new  Mesh(vertices, normals, texCoords, indices);

     return m;


     }


   void  Model::processNode(aiNode* node, const aiScene* scene){

    cout << "DEBUG:  Num of meshes: " <<  node->mNumMeshes << endl;


    for (int i=0; i < node->mNumMeshes; i++){

         aiMesh* aSmesh = scene->mMeshes[node->mMeshes[i]]; 
         Mesh* m = this->convertMesh(scene,aSmesh);
         this->meshes.push_back(m);

    }
  
   //do recursively for node's children 
   
    for(int  j = 0; j< node->mNumChildren; j++)
    {
        this->processNode(node->mChildren[j], scene);
    }


    }