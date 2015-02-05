#include "ShaderTools/DefaultRenderLoop.h"
#include <assimp/Importer.hpp> 
#include "ShaderTools/RenderPass.h"
#include "Compression/TextureTools.h"
#include "Compression/ComputeShaderTools.h"
#include "ShaderTools/VertexArrayObjects/Plane.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"
#include "ShaderTools/VertexArrayObjects/Skybox.h"
#include "ShaderTools/VertexArrayObjects/Pyramid.h"
#include "ShaderTools/VertexArrayObjects/Model.h"
#include "WarppingGame/CubemapTexture/CubemapTexture.h"

using namespace std;
using namespace glm;

auto sp = new ShaderProgram({"/Warpping/myTest.vert", "/Warpping/myTest.frag"});


auto spSkybox = new ShaderProgram({"/Warpping/skybox.vert", "/Warpping/skybox.frag"});


auto passCube = new RenderPass(
    new Skybox(), 
    spSkybox
);

auto passQuoad= new RenderPass(
    new Plane(), 
    sp
);

auto passPyram = new RenderPass(new Pyramid(), sp);




GLuint textureHandle = TextureTools::loadTexture(RESOURCES_PATH "/bambus.jpg");
GLuint texHandle = ComputeShaderTools::generateTexture();

CubemapTexture* cubeText = new CubemapTexture();

GLuint test = cubeText->create_cube_map("/home/ivanna/git_repo/OpenGL_Framework/resources/skybox/jajlands1_rt.jpg",
    "/home/ivanna/git_repo/OpenGL_Framework/resources/skybox/jajlands1_lf.jpg",
    "/home/ivanna/git_repo/OpenGL_Framework/resources/skybox/jajlands1_up.jpg",
    "/home/ivanna/git_repo/OpenGL_Framework/resources/skybox/jajlands1_dn.jpg",
    "/home/ivanna/git_repo/OpenGL_Framework/resources/skybox/jajlands1_bk.jpg",
    "/home/ivanna/git_repo/OpenGL_Framework/resources/skybox/jajlands1_ft.jpg"    
 );


float gScale = 0.5;
float lScale =0.0;
float lum = 0.5;
float horizAngle = 0.0;
float moveSpeed = 0.0;
float z= 0.0; 
float verticAngle = 0.0; 
double lasttime;



glm::mat4 projMat = glm::perspective(45.0f, (float)width/(float)height, 0.1f, 1000.0f);


void update(float time) {

    const GLfloat degPerSec = 2.0f;
    horizAngle+=time*degPerSec;
     while (horizAngle > 360.0f) horizAngle -= 360;

}

int main(int argc, char *argv[]) {


    sp -> printUniformInfo();
    sp -> printInputInfo();
    sp -> printOutputInfo();
   


 lasttime = glfwGetTime();

    renderLoop([]{

    double  currentTime = glfwGetTime();

    update ((float)(currentTime -lasttime));
    lasttime = currentTime;

        mat4 viewC = lookAt(vec3(0,15,15), vec3(0.0f),vec3(0.0, 1.0, 0.0)); 
       
         mat4   modelC = rotate(mat4(1.0), horizAngle, vec3(0.0,1.0,0.0));
                modelC = translate (modelC, vec3(0.0, 0.1, 0.0));


        mat4 modelPyramide = scale(mat4(1), vec3(2,2,2));
        modelPyramide = translate (modelPyramide, vec3 (0,0,-3));
       

      //for skybox
        mat4 model= mat4(1.0);
        model = scale(model, vec3(10,10,10));
       
        mat4 view(1);
        view = translate(view, vec3(0.0, 0.0, moveSpeed));
        view = rotate(view, verticAngle, vec3(0.0,1.0,0.0));

    //for plane
        mat4 modelQ=mat4(1.0);
        modelQ = translate(modelQ, vec3(0.0, -1, 0.0));
      


      //  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) gScale  = glm::max(gScale - 0.001, 0.);
      //  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) gScale = glm::min(gScale + 0.001, 1.);
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) lum  = glm::max(lum - 0.001, 0.);
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) lum = glm::min(lum + 0.001, 1.);

       if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);


       if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) z += 0.01;
       if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) z -=  0.01;

    
           if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) moveSpeed += 0.05;
       if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) moveSpeed -=  0.05;

       

       if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) verticAngle -= 0.01;
       if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) verticAngle +=  0.01;

       if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {


        viewC = glm::lookAt( glm::vec3(0,10,10), // Camera is at (0,10,10), in World Space
                                         glm::vec3(0,0,0), // and looks at the origin
                                         glm::vec3(0,1,0));

         }


         if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) gScale  = gScale - 0.01;
         if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) gScale = gScale + 0.01;

         if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) lScale  = lScale + 0.01;
         if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) lScale = lScale - 0.01;

  passCube
        ->  clear(0, 0, 0.3, 0)
        ->  update("uniformModel", model)
        ->  update("uniformView", view)
        ->  update("uniformProjection", projMat)
        ->  texture("tex",test)
        ->  run();
         
       

       passQuoad
     
        ->  update("uniformModel", modelQ)
        ->  update("uniformView", view)
        ->  update("uniformProjection", projMat)
        ->  update("color", vec4(0,1,0,1))
        ->  run(); 


        passPyram
      
        ->  update("uniformModel", modelPyramide)
        ->  update("uniformView", view)
        ->  update("uniformProjection", projMat)
        ->  update("color", vec4(1,0,0,1))
        ->  run();



    });


}
