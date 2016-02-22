#include "ShaderTools/Renderer.h"
#include "ShaderTools/RenderPass.h"
#include "AssetTools/Texture.h"
#include "ShaderTools/VertexArrayObjects/Grid.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "RenderTechniques/Pyramid.h"

using namespace std;
using namespace glm;

int main(int argc, char *argv[]) {
    GLFWwindow* window = generateWindow(512, 512);

    Texture* image = new Texture("/home/lochmann/tmp/blupp/out.jpg");

    auto pyramid = (new Pyramid(getWidth(window), getHeight(window),
        "/RenderTechniques/Pyramid/pullGaussian.frag",
        "/RenderTechniques/Pyramid/pushBSpline.frag"))
            ->texture("inputTex", image->getHandle());

    auto tonemapping = (new RenderPass(
        new Quad(),
        new ShaderProgram("/Filters/fullscreen.vert","/Filters/toneMapperLinear.frag")))
            ->texture("tex", pyramid->get("fragColor"));

    int level = 0;
    bool toggleInOut = true;
    setKeyCallback(window, [&] (int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            switch (key) {
            case GLFW_KEY_COMMA:
                tonemapping->update("level", (level < pyramid->getMipmapNumber())? ++level : level);
                break;
            case GLFW_KEY_PERIOD:
                tonemapping->update("level", (level > 0)? --level : level);
                break;
            case GLFW_KEY_M:
                pyramid->setLimit((level < pyramid->getMipmapNumber()-1)? ++level : level);
                break;
            case GLFW_KEY_N:
                pyramid->setLimit((level > 0)? --level : level);
                break;
            case GLFW_KEY_SPACE:
                (toggleInOut ^= 1);//? tonemapping->texture("tex", pyramid->get("fragColor")) : tonemapping->texture("tex", pyramid->get("fragPosition"));
                break;
            }
        }
    });

    render(window, [&] (float deltaTime) {
        float timer;
        float diff;
        glFinish();
        timer = glfwGetTime();

        pyramid->pull();

        if(toggleInOut)
            pyramid->push();

        glFinish();
        diff = glfwGetTime() - timer;
        printf("timer: blurring: %f\n", diff);

        tonemapping
            ->clear()
            ->run();
    });
}
