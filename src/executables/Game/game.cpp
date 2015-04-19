
#include "game.h"


using namespace glm;



 GLFWwindow* window;

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




/*          MATRIX STUFF      */
glm::mat4 projMat = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
glm:: mat4 modelCastle(1);
glm:: mat4 modelChest(1);
glm:: mat4 modelWindMill(1);
glm:: mat4 vikingModel(1);
glm:: mat4 vikingModel2(1);
glm:: mat4 followMeModel(1);
glm:: mat4 modelTree(1);
glm:: mat4 skyModel(1);
glm:: mat4 modelTerra(1);



 /* GAME LOGIC*/
float castel_y = -1.28;
vector<vec3> chestPositions;
vector<vec3> targets;
vec3 currentChestPosition;



/* TODO: pack into vector */

 RenderPassModel*  skyBoxPass;

 RenderPass*  plane ;
 RenderPassModel* trees;
 RenderPass* diffWarp;
 RenderPass* diffWarpMuelle;
 RenderPassModel* castlePass;
 RenderPassModel* chestPass;
 RenderPassModel* followMePass;
 RenderPassModel* windMillPass;
 RenderPassModel* vikingPass;
 RenderPassModel* terraPass;
 RenderPassCollector* collector;


 RenderPass* quadPass;



 Camera* camera;
 FollowObject* followMe;


 Model* myModel;
 Model* castle;
 Model* chest;
 Model* windMill;
 Model* viking;
 Model* viking2;
 Model* terraModel;
 Model* skyDom;

 vector<VertexArrayObject*> meshes;
 vector<VertexArrayObject*> castleMeshes;
 vector<VertexArrayObject*> chestMeshes;
 vector<VertexArrayObject*> windMillMeshes;
 vector<VertexArrayObject*> suzanneMeshes; 
 vector<VertexArrayObject*> vikingMeshes; 
 vector<VertexArrayObject*> vikingMeshes2; 
 vector<VertexArrayObject*> skyMeshes;
  vector<VertexArrayObject*> terraMeshes;
 vector<VertexArrayObject*> vaos;

std::map<string,ShaderProgram*> shaderPrograms;



 std::queue<glm::mat4> latencyQueue;
 int latencyFrameCount = 6;

GLuint textureHandle;
Texture* skyText;
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




 void Game::setMatrix(){

    modelCastle= glm::translate(modelCastle, glm::vec3(0,castel_y, -20));
    modelCastle = glm::scale(modelCastle, glm::vec3(0.05,0.05,0.05));
    modelChest = translate(modelChest, currentChestPosition);
    modelWindMill = translate(modelWindMill, vec3(-20, 0, 25) );
    modelWindMill = scale(modelWindMill, vec3(0.005, 0.005, 0.005));
    vikingModel = translate(vikingModel,vec3(-3, -0.01, 8) );
    vikingModel = scale(vikingModel, vec3(0.3, 0.3, 0.3));
    vikingModel2 = translate(vikingModel2,vec3(3, -0.01, 8) );
    vikingModel2 = scale(vikingModel2, vec3(0.3, 0.3, 0.3)); 
    skyModel = translate(skyModel, vec3(-2.0, -12.0, -2.0));
    skyModel =  scale(skyModel, vec3(15, 15, 15 ));
     modelTerra = translate(modelTerra, vec3(-2.0, -10.0, -2.0));
     modelTerra =  scale(modelTerra, vec3(20, 20, 20 ));

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
 
  
  diffWarpMuelle = new RenderPass(grid, warp);


   auto sp = new ShaderProgram("/Test_ShaderTools/test.vert", "/Test_ShaderTools/test.frag");
   auto sp1 = new ShaderProgram("/Warpping/myTest.vert", "/Warpping/myTest.frag");
   auto model = new ShaderProgram("/Warpping/model.vert", "/Warpping/model.frag");
   auto suzanneSp = new ShaderProgram("/Warpping/suzanne.vert", "/Warpping/suzanne.frag");
  

  this->setMatrix();


        /* Models */
   string modSP = "model";

   myModel = new Model(RESOURCES_PATH "/hemlock.3ds", modelTree, modSP);
   castle = new Model(RESOURCES_PATH "/castle.obj",modelCastle, modSP);
   chest = new Model(RESOURCES_PATH "/chest.obj", modelChest, modSP);
   windMill = new Model(RESOURCES_PATH "/windmill02.obj", modelWindMill, modSP);
   viking = new Model(RESOURCES_PATH "/viking.obj", vikingModel, "suzane");
   viking2 = new Model(RESOURCES_PATH "/viking.obj", vikingModel2, "suzane");
   skyDom = new Model(RESOURCES_PATH "/sfere.obj", skyModel, modSP);
   terraModel = new Model(RESOURCES_PATH "/terra.obj", modelTerra, modSP);

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
   vikingMeshes2 = viking2->getMeshes();
   skyMeshes = skyDom->getMeshes();
   terraMeshes = terraModel->getMeshes();

  shaderPrograms["model"] = model;
  shaderPrograms["suzane"] = suzanneSp;
  shaderPrograms["test"] = sp;


   collector = new RenderPassCollector(vaos, shaderPrograms, getWidth(window), getHeight(window));

   collector->addVAOS(castleMeshes);
   collector->addVAOS(chestMeshes);
   collector->addVAOS(windMillMeshes);
   collector->addVAOS(vikingMeshes);
   collector->addVAOS(vikingMeshes2);
   collector->addVAOS(skyMeshes);
    collector->addVAOS(terraMeshes);



     /* Render Passes */
 if (warpping){
  
    cout << "######## RENDER TO TEXTURE ###########" << endl;


   
   plane = new RenderPass( new Plane(32.0f), sp1, getWidth(window), getHeight(window));
   trees = new RenderPassModel(meshes, sp1, getWidth(window), getHeight(window));
   castlePass = new RenderPassModel(castleMeshes, model, getWidth(window), getHeight(window) );
   chestPass = new RenderPassModel(chestMeshes, model, getWidth(window), getHeight(window) );
   followMePass = new RenderPassModel(suzanneMeshes,suzanneSp, getWidth(window), getHeight(window));
   windMillPass = new RenderPassModel(windMillMeshes, model, getWidth(window), getHeight(window));
   vikingPass =  new RenderPassModel(vikingMeshes, suzanneSp, getWidth(window), getHeight(window) );
  
   diffWarp = new RenderPass(grid, warp, getWidth(window), getHeight(window) );


 }else{



   plane = new RenderPass( new Plane(32.0f), sp1);
   trees = new RenderPassModel(meshes, sp1);
   castlePass = new RenderPassModel(castleMeshes, model );
   chestPass = new RenderPassModel(chestMeshes, model );
   followMePass = new RenderPassModel(suzanneMeshes,suzanneSp);
   windMillPass = new RenderPassModel(windMillMeshes, model );
   vikingPass =  new RenderPassModel(vikingMeshes, suzanneSp );
   skyBoxPass = new RenderPassModel(skyMeshes, model);
   terraPass = new RenderPassModel(terraMeshes, model);

 }



   drawSkybox();
   glDepthMask(1);

   fillPositions();
   setChestPosition();

  // log (sp);

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

    glm::mat4 model=glm::mat4(1.0);
    glm::mat4  viewMat= getLookAt();

    simulateLanetcy (latencyFrameCount, viewMat);
     
       //use this matrix for simulating latency
    glm::mat4 viewMat_old = latencyQueue.front();


     /*   MODEL FOR followObject   */

    followMeModel = translate(followMeModel, followMe->getCurrentPosition());
    followMeModel = scale(followMeModel, vec3(0.4, 0.4, 0.4));





  
    lookAround(); 
  
    moveWithKeybord();

cout << "camera position " << camera->getPosition().x << " " << camera->getPosition().y << " " << camera->getPosition().z << endl;

if(warpping){


 cout << "######## RENDER TO TEXTURE ###########" << endl;
//  skyBoxPass
//           ->  clear (0, 0, 0,0)
//           ->  update("uniformModel", modelSkybox)
//           ->  update("uniformView", viewMat_old)
//           ->  update("uniformProjection", projMat)
//           ->  texture("tex", skybox)
//           ->  run();

// diffWarp
//         -> clear (0, 0, 0,0)
//         -> update("switchWarp", (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)?1:0)
//         -> update("viewMat", viewMat)
//         -> update("invViewProjection", glm::inverse(projMat * viewMat_old))
//         -> update("projection", projMat)
//         -> texture("colorTexture", skyBoxPass->get("fragColor"))
//         -> texture("positionTexture", skyBoxPass->get("fragPosition"))
//         -> run();

//   holefilling

//         ->texture(diffWarp->get("fragColor"))
//         ->run();

//   tonemapping
//             -> clear(0,0,0,0)
//             ->texture("tex", 
//             holefilling->getOutput())
//             ->update("resolution", getResolution(window))
//             ->run();
        
  collector
      ->  clear (0, 0, 0, 0)
      ->  update("uniformView", viewMat_old, "model")
      ->  update("uniformProjection", projMat, "model")
      ->  update("uniformView", viewMat_old, "suzane")
      ->  update("uniformProjection", projMat, "suzane")
      ->  run();

    diffWarp
        -> clear (0, 0, 0,0)
        -> update("switchWarp", (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)?1:0)
        -> update("viewMat", viewMat)
        // -> update("invViewProjection", glm::inverse(projMat * viewMat_old))
        -> update("projection", projMat)
        -> texture("colorTexture", collector->get("fragColor"))
        -> texture("positionTexture", collector->get("fragPosition"))
        -> run();
  
  holefilling
        ->texture(diffWarp->get("fragColor"))
        ->run();

  tonemapping
            -> clear(0,0,0,0)
            ->texture("tex", 
            holefilling->getOutput())
            ->update("resolution", getResolution(window))
            ->run();
 



}else {
 


skyBoxPass
        ->  clear (0.2, 0.2, 0.7, 1)
        ->  update("uniformModel", skyModel)
        ->  update("uniformView", viewMat_old)
        ->  update("uniformProjection", projMat)
        ->  texture("tex", skybox)
        ->  run();
cout<< "RESOLUTION: " << getResolution(window).x << "  " << getResolution(window).y << endl;



 castlePass
        ->  update("uniformModel", modelCastle)
        ->  update("uniformView", viewMat_old)
        ->  update("uniformProjection", projMat)
        ->  update("lightPos", lightPosition)
        ->  update ("viewPosition", camera->getPosition())
        ->  update("shinines", shinines)
        ->  update("attenuatFactor", false)
        ->  run();



 terraPass
        ->  update("uniformModel", modelTerra)
        ->  update("uniformView", viewMat_old)
        ->  update("uniformProjection", projMat)
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
      
 // plane
 //        -> update("uniformView", viewMat_old)
 //        -> update("uniformModel",model)
 //        -> update("uniformProjection", projMat)
 //        -> texture("diffuse_text", textureHandle)
 //        -> run();



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
