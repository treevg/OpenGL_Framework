#include "Model.h"

using namespace Assimp;

 Model::Model(const string& path){
 bool loaded = loadModel(path);
    assert (loaded); 

 cout << "status " << loaded << endl;
 }

 Model::~Model(){
	for (auto mesh : this->m_meshes){
        delete mesh;
    }
 }
 

 vector<Mesh*>  Model::getMeshes() const{

     	return this->m_meshes;
     }


   vector<MeshTexture> Model::getTextures() const{

    return this->m_textures;
   }
    
   bool  Model::loadModel(const string& path){
     
      bool loaded = false;

      Importer importer;
      const aiScene* scene = importer.ReadFile( path,  aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |aiProcess_FlipUVs | aiProcess_GenNormals  );
      
      //check for errors
     if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {

     	cout << "Could not load model. ERROR: " << importer.GetErrorString() << endl;

         loaded = false;

     }else {

      cout << "Number of  meshes " <<  scene->mNumMeshes << endl;
      cout << "Number of  materials " <<  scene->mNumMaterials << endl;
      cout << "Number of  textures " <<  scene->mNumTextures << endl;
      

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



    vector<MeshTexture> Model::loadTextures(aiMaterial* material, string materialType, aiTextureType type)
    {
        vector<MeshTexture> textures;
        textures.clear();


        for(int i = 0; i < material->GetTextureCount(type); i++)
        {
            aiString pathToTexture;

            material->GetTexture(type, i, &pathToTexture);
            
            bool skip = false;

            for(int j = 0; j < this->m_textures.size(); j++)
            {
               
                aiString getPath(this->m_textures[j].path);

                if(getPath == pathToTexture)
                {
                    textures.push_back(this->m_textures[j]);

                    skip = true; 
                  
                    break;
                }
            }

            if(!skip)
            {  
             
                MeshTexture texture;

                string converted =  pathToTexture.C_Str();

                texture.id = TextureTools::loadTexture(converted);

                cout << "texture id " << texture.id << endl;
               
                texture.type = materialType;
            
                
                texture.path = converted;

                textures.push_back(texture);
               
                this->m_textures.push_back(texture);  
               
               
            }
        }
        return textures;
    }




    //for every mesh of the model

    Mesh* Model::convertMesh(const aiScene* scene, aiMesh* aSmesh){

       
    vector<vec3> vertices;
    vector<vec3> normals;
    vector<vec2> texCoords;
    vector<GLuint> indices;
    vector<MeshTexture> textures;

     
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
           if(aSmesh->mMaterialIndex >= 0){
                //get material
               

                aiMaterial* material = scene->mMaterials[aSmesh->mMaterialIndex];
               
                aiString pathD;
                 material->Get(AI_MATKEY_NAME,pathD);
                 cout<< pathD.C_Str() << endl;
                 
                //  aiString textureD;
                //  material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE ,0),textureD);
                //  cout<< textureD.C_Str() << endl;

                //   aiString textureA;
                //  material->Get(AI_MATKEY_TEXTURE(aiTextureType_AMBIENT ,0),textureA);
                //  cout<< textureA.C_Str() << endl;
                 
                // //I need colors also
                aiColor3D colorD (0.f,0.f,0.f);
                 material->Get(AI_MATKEY_COLOR_DIFFUSE,colorD);
                 cout<< "difusse color" << colorD[0] << " "<< colorD[1] << endl;

                 aiColor3D colorA (0.f,0.f,0.f);
                 material->Get(AI_MATKEY_COLOR_AMBIENT,colorA);
                 cout<<"ambient_color"<<  colorA[0] << "  "<< colorA[1] << endl;


                vector<MeshTexture> diffuse = this->loadTextures(material, "texture_diffuse", aiTextureType_DIFFUSE);
             
                if(diffuse.size()>0){

                       textures.insert(textures.end(),diffuse.begin(), diffuse.end());

                }


            vector<MeshTexture> ambient = this->loadTextures(material, "texture_ambient", aiTextureType_AMBIENT);

            if(ambient.size()>0){

                       textures.insert(textures.end(),ambient.begin(), ambient.end());

                }




            vector<MeshTexture> specular = this->loadTextures(material, "texture_specular", aiTextureType_SPECULAR);

            if(specular.size()>0){

                       textures.insert(textures.end(),specular.begin(), specular.end());

                }


            }


     Mesh* m = new  Mesh(vertices, normals, texCoords, indices, textures);

     return m;


     }


   void  Model::processNode(aiNode* node, const aiScene* scene){

    cout << "DEBUG:  Num of meshes: " <<  node->mNumMeshes << endl;


    for (int i=0; i < node->mNumMeshes; i++){

         aiMesh* aSmesh = scene->mMeshes[node->mMeshes[i]]; 
         Mesh* m = this->convertMesh(scene,aSmesh);
         this->m_meshes.push_back(m);

    }
  
   //do recursively for node's children 
   
    for(int  j = 0; j< node->mNumChildren; j++)
    {
        this->processNode(node->mChildren[j], scene);
    }


    }