#include "ShaderTools/Renderer.h"
#include "ShaderTools/RenderPass.h"
#include "AssetTools/Texture.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"

using namespace std;
using namespace glm;

int main(int argc, char *argv[]) {
    GLFWwindow* window = generateWindow();

    auto blurring = (new RenderPass(
        new Quad(), 
        new ShaderProgram("/Filters/fullscreen.vert","/Filters/boxBlur.frag"), 
        getWidth(window), 
        getHeight(window)))
            ->texture("tex", Texture::load(RESOURCES_PATH "/jpg/bambus.jpg"))
            ->update("resolution", getResolution(window));

    auto tonemapping = (new RenderPass(
        new Quad(), 
        new ShaderProgram("/Filters/fullscreen.vert","/Filters/toneMapperLinear.frag")))
            ->texture("tex", blurring->get("fragColor"))
            ->update("resolution", getResolution(window));

    int blurStrength = 1;
    float toneMin = 0.0;
    float toneMax = 1.0;

    setKeyCallback(window, [&] (int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            switch (key) {
            case GLFW_KEY_UP:
                tonemapping->update("strength", (blurStrength < 10)? ++blurStrength : blurStrength);
                break;
            case GLFW_KEY_DOWN:
                tonemapping->update("strength", (blurStrength > 0)? --blurStrength : blurStrength);
                break;
            } 
        }
    });

    render(window, [&] (float deltaTime) {
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) toneMax = clamp(toneMax * pow(2.0f, deltaTime), toneMin, 10.0f);
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) toneMax = clamp(toneMax * pow(0.5f, deltaTime), toneMin, 100.0f);

        blurring
            ->clear()
            ->update("strength", blurStrength)
            ->run();

        tonemapping
            ->clear()
            ->update("minRange", toneMin)
            ->update("maxRange", toneMax)
            ->run();
    });
}
