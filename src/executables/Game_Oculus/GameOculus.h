#ifndef GAME_OCULUS_H
#define GAME_OCULUS_H


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
#include "ShaderTools/OculusRenderer.h"
#include "AssetTools/Texture.h"


#include <map>

using namespace std;
using namespace glm; 

class  GameOculus {
public:
    	GameOculus(bool);
        ~GameOculus();
        void init();
        void drawSkybox();
        void setMatrix();
	void renderSzene();
        void close();
        void exit();
        void fillPositions();




};




#endif // GAME_OCULUS_H

