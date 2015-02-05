 
#include "game.h"



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


 RenderPass*  skyBox ;
 RenderPass*  plane ;
  RenderPass*  pyramid;

static void log(ShaderProgram* s){

  s-> printUniformInfo();
  s -> printInputInfo();
  s -> printOutputInfo();

}


  Game::Game(){
    init();
    renderSzene();

  }


  void Game::init(){

   auto sp = new ShaderProgram({"/Test_ShaderTools/test.vert", "/Test_ShaderTools/test.frag"});
   plane = new RenderPass( new Plane(), sp);
   pyramid  = new RenderPass(   new Pyramid(),  sp);

   log (sp);

 }


  void  Game::drawSkybox(){

    //implement
  }
  void  Game::renderSzene(){

   //hier render loop

    


    renderLoop([]{

      glm::mat4 projMat = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
      glm::mat4 model=glm::mat4(1.0);

      glm::mat4 viewMat     = glm::lookAt(   glm::vec3(x,1,z), // Camera is at (0,10,10), in World Space
                                         glm::vec3(0,0,0), // and looks at the origin
                                         glm::vec3(0,1,0));  // Head is up (set to 0, 1 ,0 to look upside-down)


     glm::mat4 modelPyramide= glm::scale(glm::mat4(1),glm::vec3(5,20,5));


     glm::mat4 modelS = glm::scale(glm::mat4(1), glm::vec3(10,10,10));
 
  // modelPyramide= glm::translate(modelPyramide, glm::vec3(0,0,0));
    

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) z = z+0.05;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) z = z-0.05;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) x = x-0.05;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) x = x+0.05;
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);

        plane
        -> clear(0, 0, 0, 0)
        -> update("uniformView", viewMat)
        -> update("uniformModel",model)
        -> update("uniformProjection", projMat)
        -> update("color", glm::vec4(0,1,0,1))
        -> update("scale", size)
        -> update("luminance", lum)
        -> run();

for (int i = 2; i < 50; i=i+3){
  
    for (int j = -6; j < 7; j+=12){
 
  modelPyramide= glm::translate(modelPyramide, glm::vec3(j,1,50-i));

       pyramid
        -> update("uniformView", viewMat)
        -> update("uniformModel",modelPyramide)
        -> update("uniformProjection", projMat)
        -> update("color", glm::vec4(1,0,0,1))
        -> update("scale", size)
        -> update("luminance", lum)
        -> run();
          modelPyramide = glm::mat4(1.0);

      }


}


    });


  }






int main(int argc, char *argv[]) { 

Game * g = new Game();


}