#ifndef MODEL_H
#define MODEL_H

/* this class is an interface between opengl framewarc and assimp library*/

#include <vector>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp> 
#include <assimp/scene.h> 
#include <assimp/postprocess.h> 
#include <assert.h>
#include "ShaderTools/VertexArrayObjects/Mesh.h"
#include "Compression/TextureTools.h"
#include "Compression/ComputeShaderTools.h"


using namespace std;
using namespace glm;

class Model{

public:

     Model(const string& path);
     ~Model();
     vector<Mesh*> getMeshes() const;
     vector<MeshTexture> getTextures() const;
     

private:
    vector<Mesh*> m_meshes;
    vector<MeshTexture> m_textures;
    bool loadModel(const string& path);
    void processNode(aiNode* node, const aiScene* scene);
     Mesh* convertMesh(const aiScene* scene, aiMesh* aSmesh);
     vector<MeshTexture> loadTextures(aiMaterial* material, char materialType, aiTextureType type);

	};

#endif // MODEL_H