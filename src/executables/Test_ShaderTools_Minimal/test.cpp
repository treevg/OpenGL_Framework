#include "ShaderTools/DefaultRenderLoop.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "ShaderTools/VertexArrayObjects/Plane.h"

auto sp = new ShaderProgram({"/Test_ShaderTools/test.vert", "/Test_ShaderTools/test.frag"});

auto pass = new RenderPass(
    new Plane(), 
    sp
);

float size = 0.5;
float lum = 0.5;

glm::mat4 viewMat       = glm::lookAt(   glm::vec3(0,5,50), // Camera is at (0,10,10), in World Space
                                         glm::vec3(-20,0,0), // and looks at the origin
                                         glm::vec3(0,1,0));  // Head is up (set to 0, 1 ,0 to look upside-down)

glm::mat4 projMat = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
glm::mat4 model= glm::translate(glm::mat4(1.0), glm::vec3(0,0,-30));


int main(int argc, char *argv[]) {
    sp -> printUniformInfo();
    sp -> printInputInfo();
    sp -> printOutputInfo();

    renderLoop([]{

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) size  = glm::max(size - 0.001, 0.);
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) size = glm::min(size + 0.001, 1.);
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) lum  = glm::max(lum - 0.001, 0.);
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) lum = glm::min(lum + 0.001, 1.);

        pass
        -> clear(0, 0, 0, 0)
        -> update("uniformView", viewMat)
        -> update("uniformModel",model )
        -> update("uniformProjection", projMat)
        -> update("color", glm::vec4(1,0,0,1))
        -> update("scale", size)
        -> update("luminance", lum)
        -> run();
    });
}