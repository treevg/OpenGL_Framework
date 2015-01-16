#include "ShaderTools/DefaultRenderLoop.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"
#include "ShaderTools/VertexArrayObjects/Pyramid.h"
#include "WrappingGame/Modelloader/ObjModelLoader.h"

using namespace std;
using namespace glm;

auto sp = new ShaderProgram({"/Wrapping/wrap.vert", "/Wrapping/wrap.frag"});

auto passPyramid = new RenderPass(
    new Pyramid(), 
    sp
);

auto passGround = new RenderPass(  new Quad(), sp);

float gScale = 0.0;
float lScale =0.0;
float lum = 0.5;
float horizAngle = 0.0;




glm::mat4 projMat = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

int main(int argc, char *argv[]) {

    const string path = "/home/ivanna/git_repo/OpenGL_Framework/resources/cube.obj";
    ModelLoader* loader = new ObjModelLoader(path);


    sp -> printUniformInfo();
    sp -> printInputInfo();
    sp -> printOutputInfo();

    renderLoop([]{

        mat4 model = mat4(1);
        model = translate(model, vec3(-0.3,0,0));
        mat4 view = mat4(1);
        model = glm::rotate(model, horizAngle, vec3(0,1,0));
        view = translate(view, vec3(0,0,-4));

       
        //ground
        mat4 modelG = mat4(1);
        modelG = rotate(modelG, 90.0f,vec3(1,0,0));
        

  
        mat4 viewG = mat4(1);
        viewG = rotate(modelG, 90.0f,vec3(1,0,0));
        viewG = translate(view, vec3(0+lScale,-1,-1+gScale));

        


      //  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) gScale  = glm::max(gScale - 0.001, 0.);
      //  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) gScale = glm::min(gScale + 0.001, 1.);
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) lum  = glm::max(lum - 0.001, 0.);
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) lum = glm::min(lum + 0.001, 1.);

       if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);


       if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) horizAngle += 0.01;
       if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) horizAngle -=  0.01;

       if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {


        view = glm::lookAt( glm::vec3(0,10,10), // Camera is at (0,10,10), in World Space
                                         glm::vec3(0,0,0), // and looks at the origin
                                         glm::vec3(0,1,0));

         }


         if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) gScale  = gScale - 0.01;
         if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) gScale = gScale + 0.01;

         if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) lScale  = lScale + 0.01;
         if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) lScale = lScale - 0.01;


       if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {


        passPyramid
        ->  clear(0, 0, 0, 0)
        ->  update("uniformModel", model)
        ->  update("uniformView", view)
        ->  update("uniformProjection", projMat)
        ->  update("color", vec4(1,0,0,1))
        ->  update("scale", gScale)
        ->  update("luminance", lum)
        ->  run();



       }else {
       

      passGround
        -> clear(0, 0, 0, 0)
    
     
        ->  update("color", vec4(0,1,0,1))
        ->  update("uniformModel", modelG)
        ->  update("uniformView", viewG)
        ->  update("uniformProjection", projMat)
        ->  update("luminance", lum)
        -> run();


       }

        
    });
}