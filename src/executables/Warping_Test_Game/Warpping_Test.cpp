#include "ShaderTools/Renderer.h"
#include <assimp/Importer.hpp> 
#include "ShaderTools/RenderPass.h"
#include "Compression/TextureTools.h"
#include "Compression/ComputeShaderTools.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"
#include "ShaderTools/VertexArrayObjects/Skybox.h"
#include "ShaderTools/VertexArrayObjects/Pyramid.h"
#include "WarppingGame/CubemapTexture/CubemapTexture.h"
#include "WarppingGame/Modelloader/Model.h"
#include "WarppingGame/Camera/Camera.h"

using namespace std;
using namespace glm;



Camera* camera = new Camera();
GLFWwindow* window = generateWindow();



auto sp = new ShaderProgram({"/Warpping/myTest.vert", "/Warpping/myTest.frag"});
auto myModel = new ShaderProgram({"/Warpping/model.vert", "/Warpping/model.frag"});

auto suzanneSp = new ShaderProgram({"/Warpping/suzanne.vert", "/Warpping/suzanne.frag"});
auto lighting = new ShaderProgram({"/Warpping/lighting.vert", "/Warpping/lighting.frag"});


Model* m = new Model(RESOURCES_PATH "/monkey2.obj");


vector<Mesh*> meshes = m->getMeshes();



auto passModel= new RenderPass( meshes, suzanneSp);

auto passModelWithLight= new RenderPass( meshes, lighting);

double lasttime;

vec3 lightPosition = vec3(1.2f, 1.0f, 2.0f);



glm::mat4 projMat = glm::perspective(45.0f, (float)(getWidth(window), getHeight(window)), 0.1f, 1000.0f);


 static glm::mat4 getLookAt(){

  mat4 view = camera->looksAt();

  return view;

  }


/*input from keybord */

static void  moveWithKeybord(){

camera->moveWithKey(window, 0.5f);

}

/*input from mouse */
static void  lookAround(){
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  camera->rotateWithMouse(window, getWidth(window), getHeight(window));
}



static vec3  getPosition(){
  
   return  camera->getPosition();
}



int main(int argc, char *argv[]) {



    sp -> printUniformInfo();
    sp -> printInputInfo();
    sp -> printOutputInfo();
   


    lasttime = glfwGetTime();


     render(window,  [&] (float deltaTime){


      mat4 view = getLookAt();


        mat4 modelMatrix1 =mat4(1);
    
         modelMatrix1 =  scale(modelMatrix1, vec3(0.3, 0.3, 0.3));
         modelMatrix1 = translate (modelMatrix1, vec3(0.0, 0.0,-3.0));
      
       if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
           glfwSetWindowShouldClose(window, GL_TRUE);
         delete m;
       } 

       lookAround(); 
  
    moveWithKeybord();

        passModelWithLight
        ->  clear (0.5,0.3,0.2,1)
        ->  update("uniformModel", modelMatrix1)
        ->  update("uniformView", mat4(1))
        ->  update("uniformProjection", projMat)
        ->  update("lightPos", lightPosition)
        ->  update("cameraPos", (0,0,0))
        ->  update("color", vec4(0,1,0,1))
        ->  runModel();

   /*    passModel
  
        ->  update("uniformModel", translate (mat4(1), vec3(2.0, 0.0,-3.0)))
        ->  update("uniformView", view)
        ->  update("uniformProjection", projMat)
        ->  runModel(); */

    });

   
}
