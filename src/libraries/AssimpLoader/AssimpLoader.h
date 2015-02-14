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
        Assimp::Importer     _importer;
        std::string          _loadedFile;
        const aiScene*       _scene;

        glm::mat4            modelMatrix;
        Mesh*               firstMesh;

        void processScene();
        void processMesh(aiMesh* mesh);

    public:
        AssimpLoader();
        AssimpLoader* loadDAEFile(std::string filename);
        AssimpLoader* printLog();
        Mesh* getMesh();
        inline glm::mat4 getModelMatrix(){ return modelMatrix; };
};

#endif
