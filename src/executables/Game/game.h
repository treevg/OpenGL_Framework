#ifndef GAME_H
#define GAME_H

#include "ShaderTools/Renderer.h"
#include "ShaderTools/RenderPassCollector.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Pyramid.h"
#include "ShaderTools/VertexArrayObjects/Skybox.h"
#include "ShaderTools/VertexArrayObjects/Plane.h"
#include "WarppingGame/CubemapTexture/CubemapTexture.h"
#include "ShaderTools/RenderPassCollector.h"
#include "WarppingGame/Camera/Camera.h"
#include "WarppingGame/Modelloader/Model.h"
#include "WarppingGame/FollowObject/FollowObject.h"
#include "AssetTools/Texture.h"
#include "RenderTechniques/HierarchicalHoleFilling.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "ShaderTools/RenderPassModel.h"
#include "ShaderTools/RenderPassCollector.h"
#include <queue>
#include <cstdlib>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include "ShaderTools/VertexArrayObjects/Grid.h"
#include "AssetTools/Texture.h"


#include <map>

using namespace std;
using namespace glm; 

class Game {
public:
    	Game(bool);
        ~Game();
        void init();
        void drawSkybox();
        void setMatrix();
	    void renderSzene();
        void close();
        void exit();
     //   void setChestPosition();
        void fillPositions();
        

 //RenderPass* skyBox ;
 //RenderPass* plane ;
 //RenderPass* pyramid;



};




#endif // GAME_H

