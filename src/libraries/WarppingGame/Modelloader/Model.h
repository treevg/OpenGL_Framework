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
#include "ShaderTools/VertexArrayObject.h"
#include "Compression/TextureTools.h"
#include "Compression/ComputeShaderTools.h"


using namespace std;
using namespace glm;

class Model{

public:

     Model(const string& path, mat4 modelMatrix, string shaderProgramKey);
     ~Model();
     vector<VertexArrayObject*> getMeshes() const;
     vector<MeshTexture> getTextures() const;
     mat4 getModelMatrix() const;
     

private:
    vector<VertexArrayObject*> m_meshes;
    mat4 modelMatrix;
    vector<MeshTexture> m_textures;
    string shaderProgramKey;
    bool loadModel(const string& path);
    void processNode(aiNode* node, const aiScene* scene);
     Mesh* convertMesh(const aiScene* scene, aiMesh* aSmesh);
     vector<MeshTexture> loadTextures(aiMaterial* material, char materialType, aiTextureType type);

	};

#endif // MODEL_H
