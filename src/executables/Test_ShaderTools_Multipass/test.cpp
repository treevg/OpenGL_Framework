#include "ShaderTools/DefaultRenderLoop.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"

auto quadVAO = new Quad();
auto singleColorSP = new ShaderProgram({"/Test_ShaderTools/test.vert", "/Test_ShaderTools/test.frag"});

auto pass1 = new RenderPass(
    quadVAO, 
    singleColorSP,
    width, height);

auto pass2 = new RenderPass(
    quadVAO, 
    singleColorSP,
    width, height);

auto pass3 = new RenderPass(
    quadVAO, 
    singleColorSP,
    width, height);

auto compSP = new ShaderProgram({"/Test_ShaderTools/test.vert", "/Test_ShaderTools/compositing.frag"});
auto compositing = new RenderPass(
    quadVAO, 
    compSP);

float lum1 = 0.5;
float lum2 = 0.5;
float lum3 = 0.5;

int main(int argc, char *argv[]) {
    singleColorSP->printUniformInfo();
    singleColorSP->printInputInfo();
    singleColorSP->printOutputInfo();

    renderLoop([]{
        float delta = 0.0;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) delta = 0.01;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) delta = -0.01;
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) lum1 = glm::clamp(lum1 + delta, 0.0f, 1.0f);
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) lum2 = glm::clamp(lum2 + delta, 0.0f, 1.0f);
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) lum3 = glm::clamp(lum3 + delta, 0.0f, 1.0f);

        pass1
        -> clear(0, 0, 0, 0)
        -> update("color", glm::vec4(1,0,0,1))
        -> update("scale", 0.9)
        -> update("luminance", lum1)
        -> run();

        pass2
        -> clear(0, 0, 0, 0)
        -> update("color", glm::vec4(0,1,0,1))
        -> update("scale", 0.9)
        -> update("luminance", lum2)
        -> run();

        pass3
        -> clear(0, 0, 0, 0)
        -> update("color", glm::vec4(0,0,1,1))
        -> update("scale", 0.9)
        -> update("luminance", lum3)
        -> run();

        compositing
        -> clear(0, 0, 0, 0)
        -> texture("tex1", pass1->get("fragColor"))
        -> texture("tex2", pass2->get("fragColor"))
        -> texture("tex3", pass3->get("fragColor"))
        -> update("scale", 0.9)
        -> run();
    });
}