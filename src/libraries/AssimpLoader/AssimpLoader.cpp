#include "AssimpLoader.h"

AssimpLoader::AssimpLoader()
{
}

void AssimpLoader::printLog()
{

}

bool AssimpLoader::loadDAEFile(std::string filename)
{
    _scene = _importer.ReadFile(filename,
                        aiProcess_Triangulate |
                        aiProcess_SortByPType |
                        aiProcess_JoinIdenticalVertices);

    std::cout << "Scene: " << filename << std::endl;
    std::cout << " └ Children:  " << _scene->mRootNode->mNumChildren << std::endl;
    std::cout << " └ Meshcount: " << _scene->mNumMeshes << std::endl;

    processScene();
}

void AssimpLoader::processScene()
{
    for(unsigned int i = 0; i < _scene->mRootNode->mNumChildren; i++)
    {
        // Get location, scale and rotation of the root node
        aiVector3D aiRootPosition, aiRootScale;
        aiQuaternion aiRootRotation;
        _scene->mRootNode->mTransformation.Decompose(aiRootScale, aiRootRotation, aiRootPosition);

        // Get location, scale and rotation
        aiVector3D aiPosition, aiScale;
        aiQuaternion aiRotation;
        _scene->mRootNode->mChildren[i]->mTransformation.Decompose(aiScale, aiRotation, aiPosition);

        glm::mat4 modelMatrix;
        glm::translate(modelMatrix, glm::vec3(aiPosition.x, aiPosition.z, aiPosition.y) +
                                    glm::vec3(aiRootPosition.x, aiRootPosition.z, aiRootPosition.y));
    }

    aiMesh* mesh = _scene->mMeshes[0];
    std::cout << _scene->mMeshes[0]->mNumVertices << std::endl;
    processMesh(mesh);
}

GLuint AssimpLoader::getVAO(unsigned int position)
{
    aiMesh* mesh = _scene->mMeshes[0];
    GLuint vao = createVAO();
    processMesh(mesh);
    return vao;
}

void AssimpLoader::processMesh(aiMesh* mesh)
{
    std::vector<GLfloat> vertices;
    std::vector<GLint>   indices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> uvs;

    if(mesh->HasPositions())
    {
        for(unsigned int v = 0; v < mesh->mNumVertices; ++v)
        {
            vertices.push_back(mesh->mVertices[v].x);
            vertices.push_back(mesh->mVertices[v].y);
            vertices.push_back(mesh->mVertices[v].z);

            if(mesh->HasNormals())
            {
                normals.push_back(mesh->mNormals[v].x);
                normals.push_back(mesh->mNormals[v].y);
                normals.push_back(mesh->mNormals[v].z);
            }
            else
                continue;
        }
    }
    else
        std::cout << "Mesh has no positions!" << std::endl;

    if(mesh->HasFaces())
    {
        for(unsigned int f=0; f < mesh->mNumFaces; f++)
        {
            for(unsigned int i=0; i < mesh->mFaces[f].mNumIndices; i++)
                indices.push_back(mesh->mFaces[f].mIndices[i]);
        }
    }
    else
        std::cout << "Mesh has no faces!" << std::endl;

    std::cout << "Mesh " << mesh->mName.C_Str() << std::endl;
    std::cout << " └ Vertexcount " << mesh->mNumVertices << std::endl;
    std::cout << " └ Facecount   " << mesh->mNumFaces << std::endl;

    createVertexBuffer(vertices);
    createIndexBuffer(indices);

}
 
GLuint AssimpLoader::createVAO()
{
    //! Create vertex array object
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    return vertexArrayObject;
}

GLuint AssimpLoader::createVertexBuffer(std::vector<GLfloat> vertices)
{
    //! Create vertex buffer
    GLuint bufferVertices;
    glGenBuffers(1, &bufferVertices);
    glBindBuffer(GL_ARRAY_BUFFER, bufferVertices);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);
}

GLuint AssimpLoader::createIndexBuffer(std::vector<GLint> indices)
{
    //! Create index buffer
    GLuint bufferIndices;
    glGenBuffers(1, &bufferIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLint), &indices[0], GL_STATIC_DRAW);
}

GLuint AssimpLoader::createNormalBuffer(std::vector<GLfloat> normals)
{
    //! Create normal buffer
    GLuint bufferNormals;
    glGenBuffers(1, &bufferNormals);
    glBindBuffer(GL_ARRAY_BUFFER, bufferNormals);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(1);
}

GLuint AssimpLoader::createUVBuffer(std::vector<GLfloat> uvs)
{
    //! Create uv buffer
    GLuint bufferUV;
    glGenBuffers(1, &bufferUV);
    glBindBuffer(GL_ARRAY_BUFFER, bufferUV);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(GLfloat), &uvs[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(2);
}