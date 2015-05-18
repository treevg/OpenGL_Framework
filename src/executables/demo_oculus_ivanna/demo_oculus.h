#ifndef DEMO_OCULUS_H
#define DEMO_OCULUS_H



#include "ShaderTools/RenderPass.h"
#include "ShaderTools/RenderPassCollector.h"
#include "WarppingGame/Camera/Camera.h"
#include "WarppingGame/Modelloader/Model.h"
#include "WarppingGame/FollowObject/FollowObject.h"
#include "AssetTools/Texture.h"
#include "RenderTechniques/HierarchicalHoleFilling.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "ShaderTools/RenderPassModel.h"
#include <queue>
#include <cstdlib>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include "ShaderTools/VertexArrayObjects/Grid.h"
#include "AssetTools/Texture.h"
#include "ShaderTools/OculusRenderer.h"


#include <map>

using namespace std;
using namespace glm; 

class Demo_Oculus {
public:
    	Demo_Oculus();
        ~Demo_Oculus();
        void init();
  
        void setMatrix();
	void renderSzene();
        void close();
        void exit();        





};




#endif // DEMO_OCULUS_H

