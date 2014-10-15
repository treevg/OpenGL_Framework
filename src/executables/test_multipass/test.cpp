#include "ShaderTools/DefaultRenderLoop.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"

auto pass0 = new RenderPass(
    new Quad(), 
    new ShaderProgram({"test.vert", "test.frag"}));

auto pass1 = new RenderPass(
    new Quad(), 
    new ShaderProgram({"test.vert", "test.frag"}));

float size = 0.5;
float lum = 0.5;

int main(int argc, char *argv[]) {
    pass0->autoGenerateFrameBufferObject(width, height);

    renderLoop([]{
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) size  = std::max(size - 0.001, 0.);
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) size = std::min(size + 0.001, 1.);
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) lum  = std::max(lum - 0.001, 0.);
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) lum = std::min(lum + 0.001, 1.);

        pass0
        -> update("color", glm::vec4(1,0,0,1))
        -> update("scale", size)
        -> update("luminance", lum)
        -> run();

        pass1
        -> update("color", glm::vec4(1,0,0,1))
        -> update("scale", size)
        -> update("luminance", lum)
        -> run();
        // -> update("inTex", pass0->get("color"))
        // -> run();
    });
}