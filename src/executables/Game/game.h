#ifndef GAME_H
#define GAME_H

#include "ShaderTools/DefaultRenderLoop.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Pyramid.h"
#include "ShaderTools/VertexArrayObjects/Skybox.h"
#include "ShaderTools/VertexArrayObjects/Plane.h"
#include "WarppingGame/CubemapTexture/CubemapTexture.h"
#include  "WarppingGame/Camera/Camera.h"


using namespace std;
using namespace glm; 

class Game {
public:
	Game();
        ~Game() = default;
        void init();
        void drawSkybox();
	void renderSzene();
        void close();

 //RenderPass* skyBox ;
 //RenderPass* plane ;
 //RenderPass* pyramid;
Camera* cam;


};




#endif // GAME_H

