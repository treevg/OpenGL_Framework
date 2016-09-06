#include <iostream>
#include "Mesh.h"

Mesh::Mesh(aiMesh* mesh) {
	setMode(GL_TRIANGLES);

	GLuint vbo[4];
	glGenBuffers(4, vbo);
	glGenVertexArrays(1, &vertexArrayObjectHandle);
	glBindVertexArray(vertexArrayObjectHandle);

	int vertexCount = mesh->mNumVertices;
	count = mesh->mNumFaces * 3;

	GLfloat* positions = new GLfloat[vertexCount * 3];
	GLfloat* normals = new GLfloat[vertexCount * 3];
	GLfloat* uvs = new GLfloat[vertexCount * 2];
	GLuint* indices = new GLuint[count];

	int current_p = 0;
	int current_n = 0;
	int current_uv = 0;
	int current_i = 0;

	for(int i = 0; i < mesh->mNumVertices; i++) {
		positions[current_p++] = mesh->mVertices[i].x;
		positions[current_p++] = mesh->mVertices[i].y;
		positions[current_p++] = mesh->mVertices[i].z;
	}

	for(int i = 0; i < mesh->mNumVertices; i++) {
		uvs[current_uv++] = mesh->mTextureCoords[0][i].x;
		uvs[current_uv++] = mesh->mTextureCoords[0][i].y;
	}

	for(int i = 0; i < mesh->mNumVertices; i++) {
		normals[current_n++] = mesh->mNormals[i].x;
		normals[current_n++] = mesh->mNormals[i].y;
		normals[current_n++] = mesh->mNormals[i].z;
	}

	for(int i = 0; i < mesh->mNumFaces; i++) {
		indices[current_i++] = mesh->mFaces[i].mIndices[0];
		indices[current_i++] = mesh->mFaces[i].mIndices[1];
		indices[current_i++] = mesh->mFaces[i].mIndices[2];
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * vertexCount * sizeof(GLfloat), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 2 * vertexCount * sizeof(GLfloat), uvs, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, 3 * vertexCount * sizeof(GLfloat), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	delete[] positions;
	delete[] normals;
	delete[] uvs;
	delete[] indices;
}

Mesh::Mesh(std::string path, int aiPostProcessSteps) {
	mode = GL_TRIANGLES;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile( path, aiPostProcessSteps);

	if(!scene) {
		std::cerr << "ERROR LOADING " << path << std::endl;
	} else {
		std::cerr << "SUCCESS LOADING " << path << std::endl;
		dumpSceneToVAO(scene);
	}
}

Mesh::Mesh(std::string path) {
	mode = GL_TRIANGLES;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile( path,
		aiProcess_CalcTangentSpace       |
		aiProcess_GenNormals             |
		aiProcess_Triangulate            |
		aiProcess_JoinIdenticalVertices  |
		aiProcess_SortByPType);

	if(!scene) {
		std::cerr << "ERROR LOADING " << path << std::endl;
	} else {
		std::cerr << "SUCCESS LOADING " << path << std::endl;
		dumpSceneToVAO(scene);
	}
}

void Mesh::dumpSceneToVAO(const aiScene *scene) {
	GLuint vbo[4];
	glGenBuffers(4, vbo);
	glGenVertexArrays(1, &vertexArrayObjectHandle);
	glBindVertexArray(vertexArrayObjectHandle);

	count = 0;
	GLuint vertexCount = 0;
	for(int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[i];
		count += mesh->mNumFaces * 3;
		vertexCount += mesh->mNumVertices;
	}

	GLfloat* positions = new GLfloat[vertexCount * 3];
	GLfloat* normals = new GLfloat[vertexCount * 3];
	GLfloat* uvs = new GLfloat[vertexCount * 2];
	GLuint* indices = new GLuint[count];

	int current_p = 0;
	int current_n = 0;
	int current_uv = 0;
	int current_i = 0;


	for(int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[i];


		for(int i = 0; i < mesh->mNumVertices; i++) {
			positions[current_p++] = mesh->mVertices[i].x;
			positions[current_p++] = mesh->mVertices[i].y;
			positions[current_p++] = mesh->mVertices[i].z;
		}

		for(int i = 0; i < mesh->mNumVertices; i++) {
			uvs[current_uv++] = mesh->mTextureCoords[0][i].x;
			uvs[current_uv++] = mesh->mTextureCoords[0][i].y;
		}

		for(int i = 0; i < mesh->mNumVertices; i++) {
			normals[current_n++] = mesh->mNormals[i].x;
			normals[current_n++] = mesh->mNormals[i].y;
			normals[current_n++] = mesh->mNormals[i].z;
		}

		for(int i = 0; i < mesh->mNumFaces; i++) {
			indices[current_i++] = mesh->mFaces[i].mIndices[0];
			indices[current_i++] = mesh->mFaces[i].mIndices[1];
			indices[current_i++] = mesh->mFaces[i].mIndices[2];
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * vertexCount * sizeof(GLfloat), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 2 * vertexCount * sizeof(GLfloat), uvs, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, 3 * vertexCount * sizeof(GLfloat), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	delete[] positions;
	delete[] normals;
	delete[] uvs;
	delete[] indices;
}

Mesh::~Mesh() {

}

void Mesh::draw() {
	glBindVertexArray(vertexArrayObjectHandle);
	glDrawElements(mode, count, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}
