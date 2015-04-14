#include "ShaderTools/Renderer.h"
#include "ShaderTools/RenderPass.h"
#include "Compression/TextureTools.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include <typeinfo>

using namespace std;
using namespace glm;

int main(int argc, char *argv[]) {
    GLFWwindow* window = generateWindow();

    int blurStrength = 1;
    RenderPass* blurring = (new RenderPass(
        new Quad(), 
        new ShaderProgram("/Filters/fullscreen.vert","/Filters/boxBlur.frag"), 
        getWidth(window), 
        getHeight(window)))
            ->texture("tex", TextureTools::loadTexture(RESOURCES_PATH "/jpg/bambus.jpg"))
            ->update("width", getWidth(window))
            ->update("height", getHeight(window))
            ->update("strength", blurStrength);

    float toneMin = 0.0;
    float toneMax = 1.0;
    RenderPass* tonemapping = (new RenderPass(
        new Quad(), 
        new ShaderProgram("/Filters/fullscreen.vert","/Filters/toneMapperLinear.frag")))
            ->texture("tex", blurring->get("fragColor"))
            ->update("minRange", toneMin)
            ->update("maxRange", toneMax);

    setKeyCallback(window, [&] (int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {        
            if (key == GLFW_KEY_1) { 
                cout << "Blur strength: " << blurStrength << endl;
                blurStrength = clamp(blurStrength - 1, 1, 10);
                blurring->update("strength", blurStrength);
            }
            if (key == GLFW_KEY_2) { 
                cout << "Blur strength: " << blurStrength << endl;
                blurStrength = clamp(blurStrength + 1, 1, 10);
                blurring->update("strength", blurStrength);
            }
        }
    });

    render(window, [&] (float deltaTime) {
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
            toneMax = clamp(toneMax * pow(2.0f, deltaTime), toneMin, 10.0f);
            cout << "Tone max: " << toneMax << endl;
            tonemapping->update("maxRange", toneMax);
        }

        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
            toneMax = clamp(toneMax * pow(0.5f, deltaTime), toneMin, 100.0f);
            cout << "Tone max: " << toneMax << endl;
            tonemapping->update("maxRange", toneMax);
        }

        blurring
            ->clear(0,0,1,0)
            ->run();

        tonemapping
            ->clear(0,0,1,0)
            ->run();
    });
}


