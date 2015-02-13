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
        const aiScene*       _scene;

        glm::mat4            modelMatrix;
        GLuint               vao;
        Mesh*               firstMesh;

        void printLog();
        void processScene();
        GLuint createBuffers(std::vector<GLfloat> vertices,
                             std::vector<GLint> indices,
                             std::vector<GLfloat> normals,
                             std::vector<GLfloat> uvs);
        void processMesh(aiMesh* mesh);

    public:
        AssimpLoader();
        bool loadDAEFile(std::string filename);
        Mesh* getMesh();
        inline glm::mat4 getModelMatrix(){ return modelMatrix; };
};

#endif
