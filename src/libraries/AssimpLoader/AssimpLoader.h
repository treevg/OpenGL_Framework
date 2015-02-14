#ifndef H_ASSIMPLOADER
#define H_ASSIMPLOADER

#include <iostream>
#include <vector>
#include <map>
#include <GL/glew.h>
#include <GL/gl.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"

class AssimpLoader
{
    private:
        Assimp::Importer        _importer;
        std::string             _loadedFile;
        const aiScene*          _scene;
        std::vector<Mesh*>      _meshes;
        std::vector<glm::mat4>  _modelMatrices;

        void processScene();
        void processMesh(aiMesh* mesh);

    public:
        AssimpLoader();
        AssimpLoader* loadDAEFile(std::string filename);
        AssimpLoader* printLog();
        Mesh* getMesh(unsigned int position);
        inline glm::mat4 getModelMatrix(unsigned int position){
            return _modelMatrices[position];
        };
};

#endif
