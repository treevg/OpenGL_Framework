#include "ShaderTools/Renderer.h"
#include "ShaderTools/RenderPass.h"
#include "AssetTools/Texture.h"
#include "AssetTools/Mesh.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"
#include <typeinfo>

using namespace std;
using namespace glm;

int main(int argc, char *argv[]) {
    GLFWwindow* window = generateWindow();

    int blurStrength = 0;
    float rotation = 0;
    RenderPass* object = (new RenderPass(
        new Mesh(RESOURCES_PATH "/obj/cv-logo.obj"), 
        new ShaderProgram({"/3DObject/modelViewProjection.vert","/3DObject/simpleColor.frag"}),
        getWidth(window), 
        getHeight(window)))
            ->update("model", rotate(mat4(1), rotation, vec3(0,1,0)))
            ->update("view", lookAt(vec3(0,10,-40), vec3(0,7,0), vec3(0,1,0)))
            ->update("projection", perspective(45.0f, getRatio(window), 0.1f, 100.0f));

    RenderPass* blurring = (new RenderPass(
        new Quad(), 
        new ShaderProgram({"/Filters/fullscreen.vert","/Filters/boxBlur.frag"}), 
        getWidth(window), 
        getHeight(window)))
            ->texture("tex", object->get("fragNormal"))
            ->update("width", getWidth(window))
            ->update("height", getHeight(window))
            ->update("strength", blurStrength);

    float toneMin = 0.0;
    float toneMax = 1.0;
    RenderPass* tonemapping = (new RenderPass(
        new Quad(), 
        new ShaderProgram({"/Filters/fullscreen.vert","/Filters/toneMapperLinear.frag"})))
            ->texture("tex", blurring->get("fragColor"))
            ->update("minRange", toneMin)
            ->update("maxRange", toneMax);


    setKeyCallback(window, [&] (int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {        
            if (key == GLFW_KEY_1) { 
                cout << "Blur strength: " << blurStrength << endl;
                blurStrength = clamp(blurStrength - 1, 0, 10);
                blurring->update("strength", blurStrength);
            }
            if (key == GLFW_KEY_2) { 
                cout << "Blur strength: " << blurStrength << endl;
                blurStrength = clamp(blurStrength + 1, 0, 10);
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
        cout << rotation << endl;
        object
            ->clear(0,0.2,0.5,0)
            ->update("model", rotate(mat4(1), (rotation + deltaTime > 6.283)? rotation += deltaTime - 6.283 : rotation += deltaTime, vec3(0,1,0)))
            ->run();

        blurring
            ->clear(0,0.2,0.5,0)
            ->run();

        tonemapping
            ->clear(0,0.2,0.5,0)
            ->run();
    });
}


