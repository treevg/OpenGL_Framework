#ifndef COLLISION_COLLECTION_H
#define COLLISION_COLLECTION_H

/* this class is an interface between opengl framewarc and assimp library*/

#include <vector>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include "CollisionObject.h"
#include "ShaderTools/VertexArrayObjects/Mesh.h"
#include "ShaderTools/VertexArrayObject.h"
#include "WarppingGame/Player/Player.h"


using namespace std;
using namespace glm;

class CollisionCollection{

public:

    CollisionCollection();
    ~CollisionCollection();
    void add(vector<VertexArrayObject*> meshes, mat4 modelmatrix, string key);
    void add(VertexArrayObject* mesh, mat4 modelmatrix, string key);
    void add(CollisionObject* co);
    //todo: optimize -> just objects in radius 20 or so))
    bool checkCollision(Player* player);


//test with player and return bool

     

private:
    vector<CollisionObject*> collisionObjects;

	};

#endif // COLLISION_COLLECTION_H
