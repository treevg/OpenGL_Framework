#include "AssimpLoader.h"

AssimpLoader::AssimpLoader()
{
}

AssimpLoader* AssimpLoader::printLog()
{
    std::cout << "--- ASSIMP LOADER LOG ---" << std::endl;
    std::cout << "Loaded scene file   : " << _loadedFile << std::endl
              << "  └ Materialcount : " << _scene->mNumMaterials << std::endl
              << "  └ Children      : " << _scene->mRootNode->mNumChildren << std::endl
              << "  └ Meshcount     : " << _scene->mNumMeshes << std::endl
              << std::endl;
    return this;
}

AssimpLoader* AssimpLoader::loadDAEFile(std::string filename)
{
    _loadedFile = filename;
    _scene = _importer.ReadFile(filename,
                        aiProcess_Triangulate |
                        aiProcess_SortByPType |
                        aiProcess_JoinIdenticalVertices);
    processScene();
    return this;
}

void AssimpLoader::processScene()
{
    glm::mat4 modelMatrix;
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

        glm::translate(modelMatrix, glm::vec3(aiPosition.x, aiPosition.z, aiPosition.y) +
                                    glm::vec3(aiRootPosition.x, aiRootPosition.z, aiRootPosition.y));

        _modelMatrices.push_back(modelMatrix);
    }

    for(unsigned int i=0; i < _scene->mNumMaterials; ++i)
    {
        aiColor3D color;
        aiMaterial* material = _scene->mMaterials[i];
        material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        _materialColors.push_back(glm::vec3(color.r, color.g, color.b));
    }

    for(unsigned int i=0; i < _scene->mNumMeshes; ++i)
        processMesh(_scene->mMeshes[i]);
}

void AssimpLoader::processMesh(aiMesh* mesh)
{
    std::vector<GLint>   indices;
    std::vector<GLfloat> vertices;
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
        }
    }
    else
        std::cout << "Mesh has no positions!" << std::endl;

    if(mesh->HasFaces())
    {
        for(unsigned int f=0; f < mesh->mNumFaces; f++)
            for(unsigned int i=0; i < mesh->mFaces[f].mNumIndices; i++)
                indices.push_back(mesh->mFaces[f].mIndices[i]);
    }
    else
        std::cerr << "Error: AssimLoader: Mesh has no faces!" << std::endl;

    Mesh* newMesh = new Mesh(vertices, indices, normals, uvs);
    newMesh->setMaterialIndex(mesh->mMaterialIndex);
    _meshes.push_back(newMesh);
}

Mesh* AssimpLoader::getMesh(unsigned int position)
{
    if(position < _meshes.size())
        return _meshes[position];
    else {
        std::cerr << "ERROR: mesh index out of bounds!" << std::endl;
        return new Mesh();
    }
}