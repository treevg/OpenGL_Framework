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
    std::cout << " └ Materialcount : " << _scene->mNumMaterials << std::endl;
    std::cout << " └ Children      : " << _scene->mRootNode->mNumChildren << std::endl;
    std::cout << " └ Meshcount     : " << _scene->mNumMeshes << std::endl;

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

        glm::translate(modelMatrix, glm::vec3(aiPosition.x, aiPosition.z, aiPosition.y) +
                                    glm::vec3(aiRootPosition.x, aiRootPosition.z, aiRootPosition.y));
    }

    aiMesh* mesh = _scene->mMeshes[0];
    processMesh(mesh);
}

void AssimpLoader::processMesh(aiMesh* mesh)
{
    std::cout << "\n  Mesh " << mesh->mName.C_Str() << std::endl;
    std::cout << "  > Vertexcount   : " << mesh->mNumVertices << std::endl;
    std::cout << "  > Materialindex : " << mesh->mMaterialIndex << std::endl;
    std::cout << "  > Vertexcount   : " << mesh->mNumVertices << std::endl;
    std::cout << "  > Facecount     : " << mesh->mNumFaces << std::endl;

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
        {
            for(unsigned int i=0; i < mesh->mFaces[f].mNumIndices; i++)
                indices.push_back(mesh->mFaces[f].mIndices[i]);
        }
    }
    else
        std::cout << "Mesh has no faces!" << std::endl;

    std::cout << "  > Indexcount: " << indices.size() << std::endl;

    firstMesh = new Mesh(vertices, indices, normals, uvs);
}

Mesh* AssimpLoader::getMesh()
{
    return firstMesh;
}