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

class AssimpLoader
{
    private:
        Assimp::Importer     _importer;
        const aiScene*       _scene;
        GLuint               vao;
        std::map<unsigned int, std::vector<GLuint>> vaoList;

        void printLog();
        void processScene();
        GLuint createBuffers(std::vector<GLfloat> vertices,
                             std::vector<GLint> indices,
                             std::vector<GLfloat> normals,
                             std::vector<GLfloat> uvs);
        void processMesh(aiMesh* mesh);

        // OpenGL buffer creation
        GLuint createVAO();
        GLuint createVertexBuffer(std::vector<GLfloat> vertices);
        GLuint createIndexBuffer(std::vector<GLint> indices);
        GLuint createNormalBuffer(std::vector<GLfloat> normals);
        GLuint createUVBuffer(std::vector<GLfloat> uvs);

    public:
        AssimpLoader();
        bool loadDAEFile(std::string filename);
        GLuint getVAO(unsigned int position);
};

#endif
