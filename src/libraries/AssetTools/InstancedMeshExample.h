#ifndef InstancedMeshExample_H
#define InstancedMeshExample_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <ShaderTools/VertexArrayObject.h>


class InstancedMeshExample : public VertexArrayObject
{
public:
    InstancedMeshExample(std::string path);
    InstancedMeshExample(std::string path, int aiPostProcessSteps);
    ~InstancedMeshExample();

	int count;
	void draw();

    GLuint getHandle();

    int num_balls;
	
private:
	void dumpSceneToVAO(const aiScene *scene);
};

#endif // InstancedMeshExample_H
