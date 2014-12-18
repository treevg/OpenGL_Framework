#include "ShaderTools/DefaultRenderLoop.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "Compression/TextureTools.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"

using namespace std;
using namespace glm;

RenderPass* blurring = new RenderPass(new Quad(), new ShaderProgram({"/Filters/fullscreen.vert","/Filters/boxBlur.frag"}), width, height);
RenderPass* tonemapping = new RenderPass(new Quad(), new ShaderProgram({"/Filters/fullscreen.vert","/Filters/toneMapperLinear.frag"}));

int blurStrength = 1;
float toneMin = 0.0;
float toneMax = 1.0;

int texHandle = TextureTools::loadTexture(RESOURCES_PATH "/bambus.jpg");

int main(int argc, char *argv[]) {

    renderLoop([]{
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            blurStrength = clamp(blurStrength - 1, 1, 10);
            cout << "Blur strength: " << blurStrength << endl;
        }

        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
            blurStrength = clamp(blurStrength + 1, 1, 10);
            cout << "Blur strength: " << blurStrength << endl;
        } 
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
            toneMax = clamp(toneMax * 0.9f, toneMin, 10.0f);
            cout << "Tone max: " << toneMax << endl;
        }

        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
            toneMax = clamp(toneMax * 1.1f, toneMin, 100.0f);
            cout << "Tone max: " << toneMax << endl;
        } 

        blurring
            ->clear(0,0,0,0)
            ->texture("tex", texHandle)
            ->update("width", width)
            ->update("height", height)
            ->update("strength", blurStrength)
            ->run();

        tonemapping
            ->clear(0,0,0,0)
            ->texture("tex", blurring->get("fragColor"))
            ->update("minRange", toneMin)
            ->update("maxRange", toneMax)
            ->run();
    });
}

