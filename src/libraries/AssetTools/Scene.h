#ifndef SCENE_H
#define SCENE_H

#include "Mesh.h"

class Scene
{
public:
	Scene(std::string path);
	Scene(std::string path, int aiPostProcessSteps);
	~Scene();

	Mesh* get(std::string name);
	void draw();

	std::vector<Mesh*> meshes;
};
#endif // SCENE_H
