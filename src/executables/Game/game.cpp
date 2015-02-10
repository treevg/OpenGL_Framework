 
#include "game.h"

using namespace glm;

/*RenderPass* skyBox;
RenderPass* plane;
RenderPass* pyramid;
Camera* camera;
*/

//helpers


float z=50.0;
float x=0.0;
float size = 0.5;
float lum = 0.5;
float speed = 0.1f;

 

 RenderPass*  skyBox ;
 RenderPass*  plane ;
 RenderPass*  pyramid;
 Camera* camera;

static void log(ShaderProgram* s){

  s-> printUniformInfo();
  s -> printInputInfo();
  s -> printOutputInfo();

}

static void printMatrix(mat4 matrix){

   std::cout<<"matrix:  "<<glm::to_string(matrix)<<std::endl;

}


  Game::Game(){
    init();
    renderSzene();


  }

  Game::~Game(){
   
   delete skyBox;
   delete camera;
   delete plane;
   delete pyramid;

  }


  void Game::init(){

   auto sp = new ShaderProgram({"/Test_ShaderTools/test.vert", "/Test_ShaderTools/test.frag"});
   plane = new RenderPass( new Plane(), sp);
   pyramid  = new RenderPass(   new Pyramid(),  sp);
   camera =  new Camera();

   log (sp);

 }


 

  void  Game::drawSkybox(){

    //implement
  }

  static glm::mat4 getLookAt(){

  mat4 view = camera->looksAt();

     return view;

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



  void  Game::renderSzene(){

   //render loop for game

    
 
    renderLoop([]{
     
    glm::mat4 projMat = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
    glm::mat4 model=glm::mat4(1.0);
    glm::mat4  viewMat= getLookAt();

    glm::mat4 modelPyramide;    //= glm::scale(glm::mat4(1.0),glm::vec3(10,40,10));
    glm::mat4 modelS = glm::scale(glm::mat4(1), glm::vec3(10,10,10));
   
    lookAround(); 
    moveWithKeybord();
   
      plane
        -> clear(0, 0, 0, 0)
        -> update("uniformView", viewMat)
        -> update("uniformModel",model)
        -> update("uniformProjection", projMat)
        -> update("color", glm::vec4(0,1,0,1))
        -> update("luminance", lum)
        -> run();

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

for (int i = 5; i < 50; i=i+6){
  
      for (int j = -6; j < 7; j+=12){
        
         modelPyramide= glm::translate(modelPyramide, glm::vec3(j,5.5,50-i));
         modelPyramide=   glm::scale(modelPyramide,glm::vec3(10,10,10));
         

       pyramid
        -> update("uniformView", viewMat)
        -> update("uniformModel",modelPyramide)
        -> update("uniformProjection", projMat)
        -> update("color", glm::vec4(1,0,0,1))
        -> update("luminance", lum)
        -> run();

         modelPyramide = glm::mat4(1.0);

      }
}


glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   });


  }




int main(int argc, char *argv[]) { 

Game * g = new Game();

delete g;
}