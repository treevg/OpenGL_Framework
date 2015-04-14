#include "ShaderTools/Renderer.h"
#include <assimp/Importer.hpp> 
#include "ShaderTools/RenderPassModel.h"
#include "ShaderTools/RenderPassCollector.h"
#include "ShaderTools/RenderPass.h"
#include "Compression/TextureTools.h"
#include "Compression/ComputeShaderTools.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "ShaderTools/VertexArrayObjects/Cube2.h"
#include "ShaderTools/VertexArrayObject.h"
#include "ShaderTools/VertexArrayObjects/Skybox.h"
#include "ShaderTools/VertexArrayObjects/Pyramid.h"
#include "WarppingGame/Modelloader/Model.h"
#include "WarppingGame/Camera/Camera.h"
#include "ShaderTools/VertexArrayObjects/Grid.h"
#include "ShaderTools/VertexArrayObjects/Plane.h"
#include "RenderTechniques/HierarchicalHoleFilling.h"
#include <queue>

using namespace std;
using namespace glm;


GLFWwindow* window = generateWindow();
Camera* camera = new Camera();


 std::queue<glm::mat4> latencyQueue;


glm::mat4 projMat = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);

GLuint textureHandle = TextureTools::loadTexture("grass_repeating.jpg");
GLuint textureHandle2 = TextureTools::loadTexture("w1.jpg");

auto sp = new ShaderProgram({"/Warpping/demo.vert", "/Warpping/demo.frag"});
auto myModel = new ShaderProgram({"/Warpping/model.vert", "/Warpping/model.frag"});
auto compositingSP = new ShaderProgram({"/Warpping/demo.vert", "/Warpping/comp.frag"});



/*                  WOOD HOUSE           */
Model* woodHouse = new Model(RESOURCES_PATH "/WoodHouse.obj");
vector<Mesh*> woodHouseMeshes = woodHouse->getMeshes();
RenderPassModel* passwoodHouse= new RenderPassModel( woodHouseMeshes, myModel);



//                  TEST WARPING 

auto grid = new Grid(getWidth(window), getHeight(window));
auto warp = new ShaderProgram("/Warpping/warp.vert", "/Raytracing/warp.frag");
auto spPlane = new ShaderProgram("/Warpping/myTest.vert", "/Warpping/myTest.frag");

RenderPass*  plane  = new RenderPass( new Quad(),spPlane,getWidth(window), getHeight(window)); 
//RenderPass* diffWarp  = new RenderPass(grid, warp, getWidth(window), getHeight(window));
RenderPass* diffWarp  = new RenderPass(grid, warp);

/*    RENDER COLLECTOR   */

Cube2* c;
Cube2* c1;

vector<VertexArrayObject*> cubes;

RenderPassCollector* collector ; 

/*    SOME OTHER PARAMETERS   */

double lasttime;
vec3 lightPosition = vec3(0,5,3);

/*    HOLEFILLING   */


int numMipmaps = glm::log2(glm::max<float>(getWidth(window), getHeight(window)));
 auto  quad = new Quad();
 auto holefilling = new HierarchicalHoleFilling( getResolution(window), quad);

 auto tonemapping = new RenderPass( new Quad(), new ShaderProgram("/Filters/fullscreen.vert","/Filters/toneMapperLinear.frag"));




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

static void simulateLanetcy(int frameCount, glm::mat4 viewMat){

   latencyQueue.push(viewMat);

    if (latencyQueue.size() > frameCount) {

      latencyQueue.pop();
    }
}





int main(int argc, char *argv[]) {

    sp -> printUniformInfo();
    sp -> printInputInfo();
    sp -> printOutputInfo();



cubes.clear();
mat4 mc1 = scale(mat4(1), vec3(6,6,6));
     mc1 = translate( mc1,vec3(-2,1,0));

mat4 mc2 = scale(mat4(1), vec3(6,6,6));
     mc2 = translate( mc2,vec3(2,1,0));
c = new Cube2(mc1, vec4(-1,0,0,0), textureHandle);

cubes.push_back(c);

c1 = new Cube2(mc2, vec4(-1,0,0,0), textureHandle2);

cubes.push_back(c1);
   
collector = new RenderPassCollector(cubes,sp, getWidth(window), getHeight(window));

 

     render(window, [&] (float deltaTime){
   
    lookAround(); 
  
    moveWithKeybord();


      if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
           glfwSetWindowShouldClose(window, GL_TRUE);
           
         }

     
    glm::mat4 model=rotate (glm::mat4(1.0),88.5f, vec3(1,0,0));
    model=scale (model, vec3(100,100,0));
    
    glm::mat4 house = translate(mat4(1), vec3(-2.0, -10.0,-3.0));


    glm::mat4 viewMat= getLookAt();

    simulateLanetcy (12, viewMat);


     //use this matrix for simulating latency
    glm::mat4 viewMat_old = latencyQueue.front();
  

   if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
      
    

     cout << "RENDER  House " << endl;

     plane   
        -> clear (0.5,0.3,0.2,1)
        -> update("uniformView", viewMat_old)
        -> update("uniformModel", model)
        -> update("uniformProjection", projMat)
        -> texture("diffuse_text", textureHandle)
        -> run();



  } else {
     cout << " render plane" << endl;

  /*   plane   
        -> clear (0.5,0.3,0.2,1)
        -> update("uniformView", viewMat_old)
        -> update("uniformModel", model)
        -> update("uniformProjection", projMat)
        -> texture("diffuse_text", textureHandle)
        -> run();

         diffWarp
        -> clear()
        -> update("switchWarp", (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)?1:0)
        -> update("viewMat", viewMat)
        -> update("invViewProjection", glm::inverse(projMat * viewMat_old))
        -> update("projection", projMat)
        -> texture("colorTexture", plane->get("fragColor"))
        -> texture("positionTexture", plane->get("fragPosition"))
        -> run();
  
  */
      collector
        ->  clear (0, 0, 0,0)
        ->  update("uniformView", viewMat_old)
        ->  update("uniformProjection", projMat)
        ->  run();

        diffWarp
        -> clear (0, 0, 0,0)
        -> update("switchWarp", (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)?1:0)
        -> update("viewMat", viewMat)
        -> update("invViewProjection", glm::inverse(projMat * viewMat_old))
        -> update("projection", projMat)
        -> texture("colorTexture", collector->get("fragColor"))
        -> texture("positionTexture", collector->get("fragPosition"))
        -> run();

    /*    holefilling
        ->texture(diffWarp->get("fragColor"))
        ->run();

  tonemapping
            ->texture("tex", holefilling->getOutput())
            ->update("resolution", getResolution(window))
            ->run();
  
       */

  }
           

    });

   
}
