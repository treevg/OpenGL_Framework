#include <iostream>
#include "Scene.h"


Scene::Scene(std::string path, int aiPostProcessSteps) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile( path, aiPostProcessSteps);

	if(!scene) {
		std::cerr << "ERROR LOADING " << path << std::endl;
	} else {
		std::cerr << "SUCCESS LOADING " << path << std::endl;
		for(int i = 0; i < scene->mNumMeshes; i++) {
			meshes.push_back(new Mesh(scene->mMeshes[i]));
		}
	}
}

Scene::Scene(std::string path) {
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
		for(int i = 0; i < scene->mNumMeshes; i++) {
			meshes.push_back(new Mesh(scene->mMeshes[i]));
		}
	}
}

Scene::~Scene() {

}
