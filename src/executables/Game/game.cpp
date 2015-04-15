#include <queue>
#include <cstdlib>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include "ShaderTools/VertexArrayObjects/Grid.h"
#include "AssetTools/Texture.h"
#include "game.h"
#include "RenderTechniques/HierarchicalHoleFilling.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "ShaderTools/RenderPassModel.h"

using namespace glm;



/* parameters */


float z=50.0;
float x=0.0;
float size = 0.5;
float lum = 0.5;
float speed = 0.3f;
bool targetReached = false;
GLuint skybox;
bool warpping;
double lasttime;
vec3 lightPosition = vec3(0,80,0);
float shinines = 32.0f;

 
 /* GAME LOGIC*/
float castel_y = -0.28;
vector<vec3> chestPositions;
vector<vec3> targets;
vec3 currentChestPosition;



/* TODO: pack into vector */

 RenderPass*  skyBoxPass;
 RenderPass*  plane ;
 RenderPassModel* trees;
 RenderPass* diffWarp;
 RenderPass* diffWarpMuelle;
 RenderPassModel* castlePass;
 RenderPassModel* chestPass;
 RenderPassModel* followMePass;
 RenderPassModel* windMillPass;
 RenderPassModel* vikingPass;


 RenderPass* quadPass;




 Camera* camera;
 FollowObject* followMe;


 Model* myModel;
 Model* castle;
 Model* chest;
 Model* windMill;
 Model* viking;


 vector<VertexArrayObject*> meshes;
 vector<VertexArrayObject*> castleMeshes;
 vector<VertexArrayObject*> chestMeshes;
 vector<VertexArrayObject*> windMillMeshes;
 vector<VertexArrayObject*> suzanneMeshes; 
 vector<VertexArrayObject*> vikingMeshes; 

 GLFWwindow* window;



 std::queue<glm::mat4> latencyQueue;
 int latencyFrameCount = 12;

GLuint textureHandle;
CubemapTexture* cubeText;


//holefilling

  int numMipmaps = glm::log2(glm::max<float>(getWidth(window), getHeight(window)));
  Quad* quad;

/* static methods */


  static glm::mat4 getLookAt(){

  mat4 view = camera->looksAt();

  return view;

  }


static void log(ShaderProgram* s){

  s -> printUniformInfo();
  s -> printInputInfo();
  s -> printOutputInfo();

}

static void printMatrix(mat4 matrix){

   // std::cout<<"matrix:  "<<glm::to_string(matrix)<<std::endl;

}


static void quit(){

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);

  //free all resources

}

/*input from keybord */

static void  moveWithKeybord(){

camera->moveWithKey(window, speed);
quit();

}

/*input from mouse */
static void  lookAround(){
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  camera->rotateWithMouse(window, getWidth(window), getHeight(window));
}


static void setChestPosition(){

  srand (time(NULL));

  int position = rand()%8;

  currentChestPosition = chestPositions[position];

  cout << "* Chest position *" << currentChestPosition.x << "  " << currentChestPosition.z << endl;

  }

  void Game::fillPositions(){

      chestPositions.clear();
      chestPositions.push_back(glm::vec3(-20, castel_y, -16));
      chestPositions.push_back(glm::vec3( 20, castel_y, -15));
      chestPositions.push_back(glm::vec3( 16, castel_y, -44));
      chestPositions.push_back(glm::vec3( -16, castel_y, -44));
      chestPositions.push_back(glm::vec3(-20,  castel_y, -16));
      chestPositions.push_back(glm::vec3(-24.75, castel_y, 22.34));
      chestPositions.push_back(glm::vec3( -16, castel_y, -34));
      chestPositions.push_back(glm::vec3( 16, castel_y, -34));
      

}

static void simulateLanetcy(int frameCount, glm::mat4 viewMat){

   latencyQueue.push(viewMat);

    if (latencyQueue.size() > frameCount) {

      latencyQueue.pop();
    }
}


void Game::init(){
   /* Camera  */
   window = generateWindow();

   camera =  new Camera();
   
   followMe = new FollowObject(camera, 5);
   auto  quad = new Quad();

   auto grid = new Grid(getWidth(window), getHeight(window));
  



  //Warping shader
  auto warp = new ShaderProgram("/Warpping/warp.vert", "/Raytracing/warp.frag");
 
  //diffWarpMuelle = new RenderPass(grid, warp);
  diffWarpMuelle = new RenderPass(grid, warp, getWidth(window), getHeight(window));


   auto sp = new ShaderProgram("/Test_ShaderTools/test.vert", "/Test_ShaderTools/test.frag");
   auto sp1 = new ShaderProgram("/Warpping/myTest.vert", "/Warpping/myTest.frag");
   auto model = new ShaderProgram("/Warpping/model.vert", "/Warpping/model.frag");
   auto suzanneSp = new ShaderProgram("/Warpping/suzanne.vert", "/Warpping/suzanne.frag");
   auto skyboxSp = new ShaderProgram("/Warpping/skybox.vert", "/Warpping/skybox.frag");

        /* Models */

   myModel = new Model(RESOURCES_PATH "/hemlock.3ds", mat4(1));
   castle = new Model(RESOURCES_PATH "/castle.obj", mat4(1));
   chest = new Model(RESOURCES_PATH "/chest.obj", mat4(1));
   windMill = new Model(RESOURCES_PATH "/windmill02.obj", mat4(1));
   viking = new Model(RESOURCES_PATH "/viking.obj", mat4(1));

    /* external Textures */

 textureHandle = TextureTools::loadTexture("grass_repeating.jpg");
 cubeText = new CubemapTexture();
   

     /* Meshes */

   meshes = myModel->getMeshes();
   castleMeshes = castle->getMeshes();
   chestMeshes  = chest->getMeshes();
   windMillMeshes = windMill->getMeshes();
   suzanneMeshes = followMe->getSuzanne()->getMeshes();
   vikingMeshes = viking->getMeshes();


     /* Render Passes */
 if (warpping){
  
    cout << "######## RENDER TO TEXTURE ###########" << endl;
    quadPass = new RenderPass(quad, sp1, getWidth(window), getHeight(window));
    quadPass -> texture("diffuse_text", textureHandle);



   plane = new RenderPass( new Plane(32.0f), sp1, getWidth(window), getHeight(window));
   trees = new RenderPassModel(meshes, sp1, getWidth(window), getHeight(window));
   castlePass = new RenderPassModel(castleMeshes, model, getWidth(window), getHeight(window) );
   chestPass = new RenderPassModel(chestMeshes, model, getWidth(window), getHeight(window) );
   followMePass = new RenderPassModel(suzanneMeshes,suzanneSp, getWidth(window), getHeight(window));
   windMillPass = new RenderPassModel(windMillMeshes, model, getWidth(window), getHeight(window));
   vikingPass =  new RenderPassModel(vikingMeshes, suzanneSp, getWidth(window), getHeight(window) );
   skyBoxPass = new RenderPass(new Skybox(), skyboxSp, getWidth(window), getHeight(window));
   diffWarp = new RenderPass(grid, warp);


 }else{

   plane = new RenderPass( new Plane(32.0f), sp1);
   trees = new RenderPassModel(meshes, sp1);
   castlePass = new RenderPassModel(castleMeshes, model );
   chestPass = new RenderPassModel(chestMeshes, model );
   followMePass = new RenderPassModel(suzanneMeshes,suzanneSp);
   windMillPass = new RenderPassModel(windMillMeshes, model );
   vikingPass =  new RenderPassModel(vikingMeshes, suzanneSp );
   skyBoxPass = new RenderPass(new Skybox(), skyboxSp);

 }



   drawSkybox();
   glDepthMask(1);

   fillPositions();
   setChestPosition();

   log (sp);

 }

  Game::Game(bool warp){
   
    warpping = warp;
    init();
    renderSzene();
  }


  Game::~Game(){
   
   delete skyBoxPass;
   delete camera;
   delete plane;
   delete trees;
   delete myModel;
   delete castle;
   delete chest;
   delete viking;

  }

  void  Game::drawSkybox(){
   
    skybox = cubeText->create_cube_map(
    RESOURCES_PATH "/skybox/jajlands1_rt.jpg",
    RESOURCES_PATH "/skybox/jajlands1_lf.jpg",
    RESOURCES_PATH "/skybox/jajlands1_up.jpg",
    RESOURCES_PATH "/skybox/jajlands1_dn.jpg",
    RESOURCES_PATH "/skybox/jajlands1_bk.jpg",
    RESOURCES_PATH "/skybox/jajlands1_ft.jpg"    
 );

  }



  void  Game::renderSzene(){


    lasttime = glfwGetTime();

   //render loop for game

   auto holefilling = new HierarchicalHoleFilling( getResolution(window), quad);
                       
   auto tonemapping = new RenderPass( new Quad(), new ShaderProgram("/Filters/fullscreen.vert","/Filters/toneMapperLinear.frag"));
            




    render(window,  [&] (float deltaTime){

    double currentTime = glfwGetTime();
    
    if(!targetReached){
    
    targetReached = followMe->moveToTarget(currentChestPosition.x+1 , currentChestPosition.z-2 , deltaTime);
   
    }

    lasttime = currentTime;


     /*  MATRIX STUFF */

     
    glm::mat4 projMat = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
    glm::mat4 model=glm::mat4(1.0);
    glm::mat4  viewMat= getLookAt();

       
      simulateLanetcy (latencyFrameCount, viewMat);
     
    

     //use this matrix for simulating latency
    glm::mat4 viewMat_old = latencyQueue.front();

      

      //TEST

    mat4 modelQuad = glm::translate(mat4(1), glm::vec3(0, 5, -2));

    // glm::rotate(mat4(1), 120.0f, vec3(1,0,0));


      /*   MODEL FOR TREES   */
    glm:: mat4 modelTree(1) ;


    /*   MODEL FOR CASTLE   */

    glm:: mat4 modelCastle(1);
  
    modelCastle= glm::translate(modelCastle, glm::vec3(0,castel_y, -20));
    modelCastle = glm::scale(modelCastle, glm::vec3(0.05,0.05,0.05));


     /*   MODEL FOR CHEST   */
 
     glm:: mat4 modelChest(1);
     modelChest = translate(modelChest, currentChestPosition);


       /*   MODEL FOR WINDMILL   */
    glm:: mat4 modelWindMill(1);
    modelWindMill = translate(modelWindMill, vec3(-20, 0, 25) );
    modelWindMill = scale(modelWindMill, vec3(0.005, 0.005, 0.005));
      


     /*   MODEL FOR followObject   */
    glm:: mat4 followMeModel(1);
    followMeModel = translate(followMeModel, followMe->getCurrentPosition());
    followMeModel = scale(followMeModel, vec3(0.4, 0.4, 0.4));


    /*   MODEL FOR SKYBOX   */

glm:: mat4 modelSkybox = translate(mat4(1), camera->getPosition());


     glm:: mat4 vikingModel(1);
    vikingModel = translate(vikingModel,vec3(-3, -0.01, 8) );
    vikingModel = scale(vikingModel, vec3(0.3, 0.3, 0.3));


    glm:: mat4 vikingModel2(1);
    vikingModel2 = translate(vikingModel2,vec3(3, -0.01, 8) );
    vikingModel2 = scale(vikingModel2, vec3(0.3, 0.3, 0.3));  

    lookAround(); 
  
    moveWithKeybord();

cout << "camera position " << camera->getPosition().x << " " << camera->getPosition().y << " " << camera->getPosition().z << endl;

if(warpping){



 skyBoxPass
         ->  clear (0, 0, 0,0)
         ->  update("uniformModel", modelSkybox)
         ->  update("uniformView", viewMat_old)
         ->  update("uniformProjection", projMat)
         ->  texture("tex", skybox)
         ->  run();
diffWarp
        ->  clear (0, 0, 0,0)
        -> update("switchWarp", (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)?1:0)
        -> update("viewMat", viewMat)
        -> update("invViewProjection", glm::inverse(projMat * viewMat_old))
        -> update("projection", projMat)
        -> texture("colorTexture", skyBoxPass->get("fragColor"))
        -> texture("positionTexture", skyBoxPass->get("fragPosition"))
        -> run();
        
  plane
        -> clear(0,0,0,0)
        -> update("uniformView", viewMat_old)
        -> update("uniformModel", model)
        -> update("uniformProjection", projMat)
        -> texture("diffuse_text", textureHandle)
        -> run();


    diffWarpMuelle
        -> clear()
        -> update("switchWarp", (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)?1:0)
        -> update("viewMat", viewMat)
        // -> update("invViewProjection", glm::inverse(projMat * viewMat_old))
        -> update("projection", projMat)
        -> texture("colorTexture", plane->get("fragColor"))
        -> texture("positionTexture", plane->get("fragPosition"))
        -> run();
  
  holefilling
        ->texture(diffWarpMuelle->get("fragColor"))
        ->run();

  tonemapping
            -> clear(0,0,0,0)
            ->texture("tex", 
            holefilling->getOutput())
            ->update("resolution", getResolution(window))
            ->run();
 

/*vikingPass
        ->  clear(0.2,0.3,0.4,1)
        ->  update("uniformModel", vikingModel2)
        ->  update("uniformView", viewMat_old)
        ->  update("uniformProjection", projMat)
        ->  run();

 cout << "VIKING" << endl;

  castlePass
        ->  clear(0.2,0.3,0.4,1)
        ->  update("uniformModel", modelCastle)
        ->  update("uniformView", viewMat_old)
        ->  update("uniformProjection", projMat)
        ->  run();
 cout << "CASTLE" << endl;

followMePass
        -> clear(0.2,0.3,0.4,1)
        -> update("uniformView", viewMat_old)
        -> update("uniformModel",followMeModel)
        -> update("uniformProjection", projMat)
        -> run(); 

  

windMillPass
        -> clear(0.2,0.3,0.4,1)
        ->  update("uniformModel", modelWindMill)
        ->  update("uniformView", viewMat_old)
        ->  update("uniformProjection", projMat)
        ->  run();


diffWarpMuelle
        -> clear()
        -> update("viewMat", viewMat)
        -> update("invViewProjection", glm::inverse(projMat * viewMat_old))
        -> update("projection", projMat)
        -> texture("colorTexture",  windMillPass->get("fragColor"))
        -> texture("positionTexture", windMillPass->get("fragPosition"))
        -> run();
        

quadPass
        ->  clear(0.2,0.3,0.4,1)
        ->  update("uniformModel", modelQuad)
        ->  update("uniformView", viewMat_old)
        ->  update("uniformProjection", projMat)
         -> texture("diffuse_text", textureHandle)
        ->  run();


diffWarpMuelle
       
        -> update("viewMat", viewMat)
        -> update("invViewProjection", glm::inverse(projMat * viewMat_old))
        -> update("projection", projMat)
        -> texture("colorTexture",  quadPass->get("fragColor"))
        -> texture("positionTexture", quadPass->get("fragPosition"))
        -> run();
  
   
   

 vikingPass
        ->  clear(0.2,0.3,0.4,1)
        ->  update("uniformModel", vikingModel)
        ->  update("uniformView", viewMat_old)
        ->  update("uniformProjection", projMat)
        ->  run();

 chestPass
        -> clear(0.2,0.3,0.4,1)
        ->  update("uniformModel", modelChest)
        ->  update("uniformView", viewMat_old)
        ->  update("uniformProjection", projMat)
        ->  run();


 for (int i = 6; i < 130; i=i+6){
  
      for (int j = -50; j < 51; j+=100){

        if( i == 6   ||  i == 120){

        modelTree  = translate(modelTree, vec3(j/4, 3, 70.0-i));


        }else{
       
       modelTree  = translate(modelTree, vec3(j, 3, 60.0-i));

        }
        

       modelTree=   glm::scale(modelTree,glm::vec3(2, 2 ,2));

       modelTree = rotate(modelTree, 80.0f,  vec3(1.0,0.0,0.0));
       
       
        trees
        ->  update("uniformModel", modelTree)
        ->  update("uniformView", viewMat_old)
        ->  update("uniformProjection", projMat)
        ->  run();
     
        modelTree = glm::mat4(1);
    
      }

    }


*/

/*
diffWarp
        -> clear()
        -> update("switchWarp", (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)?1:0)
        -> update("viewMat", viewMat)
        -> update("invViewProjection", glm::inverse(projMat * viewMat_old))
        -> update("projection", projMat)
        -> texture("colorTexture", plane->get("fragColor"))
        -> texture("positionTexture", plane->get("fragPosition"))
        -> run(); 



       diffWarp
        -> update("switchWarp", (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)?1:0)
        -> update("viewMat", viewMat)
        -> update("invViewProjection", glm::inverse(projMat * viewMat_old))
        -> update("projection", projMat)
        -> texture("colorTexture", vikingPass->get("fragColor"))
        -> texture("positionTexture", vikingPass->get("fragPosition"))
        -> run();


       diffWarp
        //-> clearDepth()
        -> update("viewMat", viewMat)
        -> update("invViewProjection", glm::inverse(projMat * viewMat_old))
        -> texture("colorTexture", castlePass->get("fragColor"))
        -> texture("positionTexture", castlePass->get("fragPosition"))
        -> run();


     diffWarp
        -> update("viewMat", viewMat)
        -> update("invViewProjection", glm::inverse(projMat * viewMat_old))
        -> update("projection", projMat)
        -> texture("colorTexture",  followMePass->get("fragColor"))
        -> texture("positionTexture", followMePass->get("fragPosition"))
        -> run();

*/
       

        

  /*    diffWarp
        -> update("viewMat", viewMat)
        -> update("invViewProjection", glm::inverse(projMat * viewMat_old))
        -> update("projection", projMat)
        -> texture("colorTexture",  chestPass->get("fragColor"))
        -> texture("positionTexture", chestPass->get("fragPosition"))
        -> run();

 */

}else {
 


skyBoxPass
        ->  clear (0.2, 0.2, 0.7, 1)
        ->  update("uniformModel", modelSkybox)
        ->  update("uniformView", viewMat_old)
        ->  update("uniformProjection", projMat)
        ->  texture("tex", skybox)
        ->  run();


 castlePass
        -> clearDepth()
        ->  update("uniformModel", modelCastle)
        ->  update("uniformView", viewMat_old)
        ->  update("uniformProjection", projMat)
        ->  update("lightPos", lightPosition)
        ->  update ("viewPosition", camera->getPosition())
        ->  update("shinines", shinines)
        ->  update("attenuatFactor", false)
        ->  run();


windMillPass
        ->  update("uniformModel", modelWindMill)
        ->  update("uniformView", viewMat_old)
        ->  update("uniformProjection", projMat)
        ->  update("shinines", shinines)
        ->  update ("viewPosition", camera->getPosition())
        ->  update("lightPos", lightPosition)
        ->  update("attenuatFactor", false)
        ->  run();
        
 plane
        -> update("uniformView", viewMat_old)
        -> update("uniformModel",model)
        -> update("uniformProjection", projMat)
        -> texture("diffuse_text", textureHandle)
        -> run();


 followMePass

        -> update("uniformView", viewMat_old)
        -> update("uniformModel",followMeModel)
        -> update("uniformProjection", projMat)
        -> run(); 

   vikingPass
        //-> clear(0.2,0.3,0.4,1)
        ->  update("uniformModel", vikingModel2)
        ->  update("uniformView", viewMat_old)
        ->  update("uniformProjection", projMat)
        ->  run();

   vikingPass
        ->  update("uniformModel", vikingModel)
        ->  update("uniformView", viewMat_old)
        ->  update("uniformProjection", projMat)
        ->  run();

 chestPass
        ->  update("uniformModel", modelChest)
        ->  update("uniformView", viewMat_old)
        ->  update("uniformProjection", projMat)
        ->  update ("viewPosition", camera->getPosition())
        ->  update("lightPos", lightPosition)
        ->  update("shinines", shinines)
        ->  update("attenuatFactor", false)
        ->  run();


 for (int i = 6; i < 130; i=i+6){
  
      for (int j = -50; j < 51; j+=100){

        if( i == 6   ||  i == 120){

        modelTree  = translate(modelTree, vec3(j/4, 3, 70.0-i));


        }else{
       
       modelTree  = translate(modelTree, vec3(j, 3, 60.0-i));

        }
        

       modelTree =   glm::scale(modelTree,glm::vec3(2, 2 ,2));
       modelTree = rotate(modelTree, 80.0f,  vec3(1.0,0.0,0.0));
       
       
        trees
        ->  update("uniformModel", modelTree)
        ->  update("uniformView", viewMat_old)
        ->  update("uniformProjection", projMat)
        ->  update ("viewPosition", camera->getPosition())
        ->  update("lightPos", lightPosition)
        ->  update("shinines", shinines)
        ->  update("attenuatFactor", false)
        ->  run();
     
        modelTree = glm::mat4(1);
      }
    }

}


   });

  }



int main(int argc, char *argv[]) { 

Game * g = new Game(true);

delete g;
}
