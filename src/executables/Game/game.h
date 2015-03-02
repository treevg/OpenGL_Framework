#ifndef GAME_H
#define GAME_H

#include "ShaderTools/DefaultRenderLoop.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Pyramid.h"
#include "ShaderTools/VertexArrayObjects/Skybox.h"
#include "ShaderTools/VertexArrayObjects/Plane.h"
#include "WarppingGame/CubemapTexture/CubemapTexture.h"
#include  "WarppingGame/Camera/Camera.h"
#include  "WarppingGame/Modelloader/Model.h"
#include  "WarppingGame/FollowObject/FollowObject.h"


using namespace std;
using namespace glm; 

class Game {
public:
	Game();
        ~Game();
        void init();
        void drawSkybox();
	    void renderSzene();
        void close();
        void exit();
        vec3 setChestPosition();
        void fillPositions(float);
        

 //RenderPass* skyBox ;
 //RenderPass* plane ;
 //RenderPass* pyramid;



};




#endif // GAME_H

