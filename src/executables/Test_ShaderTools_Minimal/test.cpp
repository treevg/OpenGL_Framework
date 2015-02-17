#include "ShaderTools/DefaultRenderLoop.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"

auto sp = new ShaderProgram({"/Test_ShaderTools/test.vert", "/Test_ShaderTools/test.frag"});

auto pass = new RenderPass(
    new Quad(), 
    sp
);

float size = 0.5;
float lum = 0.5;

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
        -> update("color", glm::vec4(1,0,0,1))
        -> update("scale", size)
        -> update("luminance", lum)
        -> run();
    });
}