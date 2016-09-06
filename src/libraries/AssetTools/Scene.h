#ifndef SCENE_H
#define SCENE_H

#include "Mesh.h"
#include "Texture.h"

class Object
{
public:
	Mesh* mesh;
	int texture;
};

class Scene
{
public:
	Scene(std::string path, int aiPostProcessSteps = -1);
	~Scene();

	Mesh* get(std::string name);
	void draw();

	// std::vector<Mesh*> meshes;
	std::vector<int> textures;
	std::vector<Object*> objects;

	std::string path;
	std::string texturesPath;

private:
	void loadMaterials();
	const aiScene* scene;
};
#endif // SCENE_H
