#include "Room.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Room::Room() {
	const std::string path = "D:/FP14/Framework/resources/obj/cube.obj";
	mode = GL_TRIANGLES;

	// Create an instance of the Importer class
	Assimp::Importer importer;
	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// propably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile(path,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);
	// If the import failed, report it
	if (!scene)
	{
		printf(importer.GetErrorString());

	}


	// Now we can access the file's contents.
	const int iVertexTotalSize = sizeof(aiVector3D) * 2 + sizeof(aiVector2D);
	
	int iTotalVertices = 0;

	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];
		int iMeshFaces = mesh->mNumFaces;
		//iMaterialIndices.push_back(mesh->mMaterialIndex);
		//int iSizeBefore = vboModelData.GetCurrentSize();
		//iMeshStartIndices.push_back(iSizeBefore / iVertexTotalSize);
		for (int j = 0; j < iMeshFaces; j++)
		{
			const aiFace& face = mesh->mFaces[j];
			for (int k = 0; k < 3; k++)
			{
				aiVector3D pos = mesh->mVertices[face.mIndices[k]];
				//aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[k]];
				aiVector3D normal = mesh->HasNormals() ? mesh->mNormals[face.mIndices[k]] : aiVector3D(1.0f, 1.0f, 1.0f);
				//vboModelData.AddData(&pos, sizeof(aiVector3D));
				//vboModelData.AddData(&uv, sizeof(aiVector2D));
				//vboModelData.AddData(&normal, sizeof(aiVector3D));
				vertices.insert(vertices.end(), (glm::vec3*)&pos, (glm::vec3*)&pos + sizeof(glm::vec3));
				//u.insert(u.end(), (glm::vec2*)&uv, (glm::vec2*)&uv + sizeof(glm::vec2));
				n.insert(n.end(), (glm::vec3*)&normal, (glm::vec3*)&normal + sizeof(glm::vec3));
			}
		}
		int iMeshVertices = mesh->mNumVertices;
		iTotalVertices += iMeshVertices;
		//iMeshSizes.push_back((vboModelData.GetCurrentSize() - iSizeBefore) / iVertexTotalSize);
	}
	// We're done. Everything will be cleaned up by the importer destructor

	glGenVertexArrays(1, &vertexArrayObjectHandle);
	glBindVertexArray(vertexArrayObjectHandle);

	GLuint vertexBufferHandles[2];
	glGenBuffers(2, vertexBufferHandles);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[0]);


	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[1]);
	glBufferData(GL_ARRAY_BUFFER, n.size() * sizeof(glm::vec3), &n[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}


void Room::draw() {
	glBindVertexArray(vertexArrayObjectHandle);
	glDrawArrays(mode, 0, vertices.size()*3);
}


