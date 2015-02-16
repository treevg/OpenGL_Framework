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
        std::vector<glm::vec3>  _materialColors;

        void processScene();
        void processMesh(aiMesh* mesh);

    public:
        /**
         * @brief Creates an AssimpLoader instance
         * @details 
         */
        AssimpLoader();
        
        AssimpLoader*loadFile(std::string filename);
        AssimpLoader* printLog();
        Mesh* getMesh(unsigned int position);
        inline std::vector<Mesh*>* getMeshList() {
            return &_meshes;
        }
        inline glm::mat4 getModelMatrix(unsigned int meshIndex){
            return _modelMatrices[meshIndex];
        };
        inline glm::vec3 getMaterialColor(unsigned int materialIndex){
            return _materialColors[materialIndex];
        };
};

#endif
