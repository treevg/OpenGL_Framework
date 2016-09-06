#include <iostream>
#include <dirent.h>
#include "Scene.h"

std::string dirnameOf(const std::string& fname)
{
     size_t pos = fname.find_last_of("\\/");
     return (std::string::npos == pos)
         ? ""
         : fname.substr(0, pos);
}

Scene::Scene(std::string path, int aiPostProcessSteps) {

	this->path = path;
	this->texturesPath = dirnameOf(path) + "/";

	std::cout << texturesPath << std::endl;

	Assimp::Importer importer;

	if (aiPostProcessSteps != -1) {
		// scene = importer.ReadFile( path + "/" + getFile(path, ".obj"),
		scene = importer.ReadFile(path,
			aiPostProcessSteps);
	} else {
		scene = importer.ReadFile(path,
			// aiProcess_CalcTangentSpace       |
			aiProcess_GenNormals            |
			// aiProcess_Triangulate           |
			aiProcess_JoinIdenticalVertices  |
			// aiProcess_MakeLeftHanded		|
			// aiProcess_FlipUVs				|
			aiProcess_SortByPType);
	}

	if(!scene) {
		std::cerr << "ERROR LOADING " << path << std::endl;
		return;
	} else {
		std::cerr << "SUCCESS LOADING " << path << std::endl;

		//MATERIALS
		for(int i = 0; i < scene->mNumMaterials; i++)
		{
		   const aiMaterial* material = scene->mMaterials[i];

		   aiString filename;

		   if(material->GetTexture(aiTextureType_DIFFUSE, 0, &filename) == AI_SUCCESS)
		   {
			   Texture* t = new Texture(texturesPath + filename.data);
			   textures.push_back(t->getHandle());
		   } else {
			   textures.push_back(0);
		   }
		}


		//MESHES AND OBJECTS
		for(int i = 0; i < scene->mNumMeshes; i++) {
			Object* o = new Object();
			o->mesh = new Mesh(scene->mMeshes[i]);
			o->texture = textures[scene->mMeshes[i]->mMaterialIndex];
			objects.push_back(o);
		}
	}
}

void Scene::loadMaterials() {
}

Scene::~Scene() {

}
