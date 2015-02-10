#ifndef MODEL_H
#define MODEL_H


#include <vector>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp> 
#include <assimp/scene.h> 
#include <assimp/postprocess.h> 
#include "ShaderTools/VertexArrayObjects/Mesh.h"


using namespace std;
using namespace glm;

class Model {

public:

     Model(const string& path);
     ~Model();
     vector<Mesh> getMeshes() const;
//
private:
    vector<Mesh> meshes;

    bool loadModel(const string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh convertMesh(const aiScene* scene, aiMesh* mesh);
//find out, how to load material and texture from model file and bind to opengl
    vector<Texture> loadMatTex();

	};

#endif // MODEL_H
