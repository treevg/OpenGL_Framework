#include "ShaderTools/DefaultRenderLoop.h"
#include <assimp/Importer.hpp> 
#include "ShaderTools/RenderPass.h"
#include "Compression/TextureTools.h"
#include "Compression/ComputeShaderTools.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"
#include "ShaderTools/VertexArrayObjects/Pyramid.h"
#include "ShaderTools/VertexArrayObjects/Model.h"

using namespace std;
using namespace glm;

auto sp = new ShaderProgram({"/Wrapping/wrap.vert", "/Wrapping/wrap.frag"});

auto passCube = new RenderPass(
    new Model( RESOURCES_PATH "/cube.obj"), 
    sp
);

auto passChevr = new RenderPass(  new Model (RESOURCES_PATH "/Chevrolet.obj") , sp);

GLuint textureHandle = TextureTools::loadTexture(RESOURCES_PATH "/bambus.jpg");
GLuint texHandle = ComputeShaderTools::generateTexture();

float gScale = 0.5;
float lScale =0.0;
float lum = 0.5;
float horizAngle = -40.0;
float verticAngle = 0.0; 



glm::mat4 projMat = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

int main(int argc, char *argv[]) {

  
    
    sp -> printUniformInfo();
    sp -> printInputInfo();
    sp -> printOutputInfo();

    renderLoop([]{

        mat4 viewC = lookAt(vec3(0,15,15), vec3(0.0f),vec3(0.0, 1.0, 0.0)); 
       
         mat4   modelC = rotate(mat4(1.0), horizAngle, vec3(0.0,1.0,0.0));
                modelC = translate (modelC, vec3(0.0, 0.1, 0.0));


        

        //ground
        mat4 modelG = mat4(1);
        modelG = rotate(modelG, 90.0f,vec3(1,0,0));
        

        mat4 viewG = mat4(1);
        viewG = translate(viewG, vec3(0,0,5));  

        viewG = rotate(viewG, 90.0f,vec3(1,0,0));
        viewG = translate(viewG, vec3(0+lScale,-1,-1+gScale));

        


      //  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) gScale  = glm::max(gScale - 0.001, 0.);
      //  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) gScale = glm::min(gScale + 0.001, 1.);
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) lum  = glm::max(lum - 0.001, 0.);
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) lum = glm::min(lum + 0.001, 1.);

       if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);


       if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) horizAngle += 0.01;
       if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) horizAngle -=  0.01;

       if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) verticAngle += 0.01;
       if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) verticAngle -=  0.01;

       if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {


        viewC = glm::lookAt( glm::vec3(0,10,10), // Camera is at (0,10,10), in World Space
                                         glm::vec3(0,0,0), // and looks at the origin
                                         glm::vec3(0,1,0));

         }


         if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) gScale  = gScale - 0.01;
         if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) gScale = gScale + 0.01;

         if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) lScale  = lScale + 0.01;
         if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) lScale = lScale - 0.01;


          if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {


        passCube
        ->  clear(0, 0, 0, 0)
        ->  update("uniformModel", modelC)
        ->  update("uniformView", viewC)
        ->  update("uniformProjection", projMat)
        ->  update("color", vec4(1,0,0,1))
        ->  update("scale", gScale)
        ->  update("luminance", lum)
        ->  run();



       }else {
       

      passChevr
        -> clear(0, 0, 0, 0)
    
     
        ->  update("color", vec4(0,1,0,1))
        ->  update("uniformModel", modelC)
        ->  update("uniformView", viewC)
        ->  update("uniformProjection", projMat)
        ->  update("luminance", lum)
        -> run();


       }

        
    });
}