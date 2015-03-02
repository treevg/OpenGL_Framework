#include <queue>
#include "ShaderTools/VertexArrayObjects/Grid.h"
#include "game.h"

using namespace glm;



//helpers


float z=50.0;
float x=0.0;
float size = 0.5;
float lum = 0.5;
float speed = 0.1f;
double lasttime;

 
 /* GAME LOGIC*/
float castel_y = -0.28;
vector<vec3> chestPositions;
vec3 currentChestPosition;



/* TODO: pack into vector */

 RenderPass*  skyBox ;
 RenderPass*  plane ;
 RenderPass* trees;
 RenderPass* diffWarp;
 RenderPass* castlePass;
 RenderPass* chestPass;
 RenderPass* followMePass;
 RenderPass* windMillPass;

 Camera* camera;
 Model* myModel;
 Model* castle;
 Model* chest;
 Model* windMill;


 vector<Mesh*> meshes;
 vector<Mesh*> castleMeshes;
 vector<Mesh*> chestMeshes;
 vector<Mesh*> windMillMeshes;


 FollowObject* followMe;


 std::queue<glm::mat4> latencyQueue;
 int latencyFrameCount = 6;


 GLuint textureHandle = TextureTools::loadTexture("/grass_repeating.jpg");


/* static methods */


  static glm::mat4 getLookAt(){

  mat4 view = camera->looksAt();

  return view;

  }


static void log(ShaderProgram* s){

  s-> printUniformInfo();
  s -> printInputInfo();
  s -> printOutputInfo();

}

static void printMatrix(mat4 matrix){

   std::cout<<"matrix:  "<<glm::to_string(matrix)<<std::endl;

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
  camera->rotateWithMouse(window, width, height);
}



static void simulateLanetcy(int frameCount, glm::mat4 viewMat){

   latencyQueue.push(viewMat);

    if (latencyQueue.size() > frameCount) {

      latencyQueue.pop();
    }
}


void Game::init(){
   /* Camera  */

   camera =  new Camera();
   
   followMe = new FollowObject(camera, 1.5);

  auto grid = new Grid(width,height);
  //Warping shader
  auto warp = new ShaderProgram({"/Warpping/warp.vert", "/Raytracing/warp.frag"});
  diffWarp = new RenderPass(grid, warp);

   auto sp = new ShaderProgram({"/Test_ShaderTools/test.vert", "/Test_ShaderTools/test.frag"});
   auto sp1 = new ShaderProgram({"/Warpping/myTest.vert", "/Warpping/myTest.frag"});
   auto model = new ShaderProgram({"/Warpping/model.vert", "/Warpping/model.frag"});

        /* Models */

   myModel = new Model(RESOURCES_PATH "/hemlock.3ds");
   castle = new Model(RESOURCES_PATH "/castle.obj");
   chest = new Model(RESOURCES_PATH "/chest.obj");
   windMill = new Model(RESOURCES_PATH "/windmill02.obj");
   

     /* Meshes */

   meshes = myModel->getMeshes();
   castleMeshes = castle->getMeshes();
   chestMeshes  = chest->getMeshes();
   windMillMeshes = windMill->getMeshes();


     /* Render Passes */

   plane = new RenderPass( new Plane(32.0f), sp1);
   trees = new RenderPass(meshes, sp1);
   castlePass = new RenderPass(castleMeshes, model );
   chestPass = new RenderPass(chestMeshes, model );
   followMePass = new RenderPass(followMe->getCube(),sp);
   windMillPass = new RenderPass(windMillMeshes, model );
  
    
    fillPositions();

  
   log (sp);

 }

  Game::Game(){
    init();
    renderSzene();
  }


  Game::~Game(){
   
   delete skyBox;
   delete camera;
   delete plane;
   delete trees;
   delete myModel;
   delete castle;
   delete chest;

  }

  void  Game::drawSkybox(){

    //implement
  }

 

  void Game::fillPositions(){

      chestPositions.clear();
      chestPositions.push_back(glm::vec3(-20, castel_y, -16));
      

}


  static void setChestPosition(){

    //get randomly 
  currentChestPosition = chestPositions[0];

  }

  void  Game::renderSzene(){

    lasttime = glfwGetTime();

   //render loop for game

    renderLoop([]{

    double  currentTime = glfwGetTime();
    float deltaTime = (float)currentTime-lasttime;
    followMe->moveToTarget(vec3(0,followMe->getCurrentPosition().y, 0), deltaTime);
  
    lasttime = currentTime;


     /*  MATRIX STUFF */

     
    glm::mat4 projMat = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
    glm::mat4 model=glm::mat4(1.0);
    glm::mat4  viewMat= getLookAt();

   simulateLanetcy (latencyFrameCount, viewMat);
     
     //use this matrix for simulating latency
    glm::mat4 viewMat_old = latencyQueue.front();

      

      /*   MODEL FOR TREES   */
    glm:: mat4 modelTree(1) ;


    /*   MODEL FOR CASTLE   */

    glm:: mat4 modelCastle(1);
  
    modelCastle= glm::translate(modelCastle, glm::vec3(1,castel_y, -20));
    modelCastle = glm::scale(modelCastle, glm::vec3(0.05,0.05,0.05));


     /*   MODEL FOR CHEST   */
     setChestPosition();
     glm:: mat4 modelChest(1);
     modelChest = translate(modelChest, currentChestPosition);



       /*   MODEL FOR WINDMILL   */
    glm:: mat4 modelWindMill(1);
    modelWindMill = translate(modelWindMill, vec3(-20, 0, 25) );
    modelWindMill = scale(modelWindMill, vec3(0.005, 0.005, 0.005));
      


     /*   MODEL FOR CUBE   */
    glm:: mat4 followMeModel(1);
    followMeModel = translate(followMeModel, followMe->getCurrentPosition());
    followMeModel = scale(followMeModel, vec3(0.2, 0.2, 0.2));
      

    lookAround(); 
  
    moveWithKeybord();



 castlePass
        -> clear(0.6, 0.6, 0.8, 0)
        ->  update("uniformModel", modelCastle)
        ->  update("uniformView", viewMat)
        ->  update("uniformProjection", projMat)
        ->  runModel();

windMillPass
        ->  update("uniformModel", modelWindMill)
        ->  update("uniformView", viewMat)
        ->  update("uniformProjection", projMat)
        ->  runModel();


   
 plane
 
        -> update("uniformView", viewMat)
        -> update("uniformModel",model)
        -> update("uniformProjection", projMat)
        -> texture("diffuse_text", textureHandle)
        -> run();


followMePass

        -> update("uniformView", viewMat)
        -> update("uniformModel",followMeModel)
        -> update("uniformProjection", projMat)
        -> update("color", vec4(1,0,0,1))
        -> run();

 chestPass
        ->  update("uniformModel", modelChest)
        ->  update("uniformView", viewMat)
        ->  update("uniformProjection", projMat)
        ->  runModel();



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
        ->  update("uniformView", viewMat)
        ->  update("uniformProjection", projMat)
        ->  runModel();
     
        modelTree = glm::mat4(1);
    
      }

    }



    /*   diffWarp
        -> clear(0,0,0,0)
        -> update("switchWarp", (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)?1:0)
        -> update("viewMat", viewMat)
        -> update("invViewProjection", glm::inverse(projMat * viewMat_old))
        -> update("projection", projMat)
        -> texture("colorTexture", plane->get("fragPosition"))
        -> texture("positionTexture", plane->get("fragPosition"))
        -> run(); */
   });

  }



int main(int argc, char *argv[]) { 

Game * g = new Game();

delete g;
}