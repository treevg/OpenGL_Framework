#include "ShaderTools/DefaultRenderLoop.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"

auto pass0 = new RenderPass(
    new Quad(), 
    new ShaderProgram({"test.vert", "test.frag"}),
    new FramebufferObject({
        texture("color", GL_RGBA16F, 16, 16),
        texture("depth", GL_DEPTH_COMPONENT32F​),
        texture("position", GL_RGBA32F)
    });

auto pass1 = new RenderPass(
    new Quad(), 
    new ShaderProgram({"test.vert", "test.frag"}));

float size = 0.5;
float lum = 0.5;

int main(int argc, char *argv[]) {
    start([]{
        if (glfwGetKey(defaultRenderLoop->window, GLFW_KEY_UP) == GLFW_PRESS) size += 0.001;
        if (glfwGetKey(defaultRenderLoop->window, GLFW_KEY_DOWN) == GLFW_PRESS) size -= 0.001;
        if (glfwGetKey(defaultRenderLoop->window, GLFW_KEY_RIGHT) == GLFW_PRESS) lum  = std::min(lum - 0.001, 1.);
        if (glfwGetKey(defaultRenderLoop->window, GLFW_KEY_LEFT) == GLFW_PRESS) lum = std::max(lum + 0.001, 0.);

        pass0
        -> update("color", glm::vec4(1,0,0,1))
        -> update("scale", size)
        -> update("luminance", lum)
        -> run();

        pass1
        -> update("inTex", pass0->get("color"))
        -> run();
    });
}