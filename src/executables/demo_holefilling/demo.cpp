#include "ShaderTools/Renderer.h"
#include "AssetTools/Texture.h"
#include "ShaderTools/VertexArrayObjects/Grid.h"
#include "RenderTechniques/HierarchicalHoleFilling.h"

using namespace std;
using namespace glm;

int main(int argc, char *argv[]) {
    GLFWwindow* window = generateWindow();

    float rotX = 0.785f;
    float rotY = 0.0f;
    int numMipmaps = glm::log2(glm::max<float>(getWidth(window), getHeight(window)));
    auto quad = new Quad();

    auto sparse = (new RenderPass(
        new Grid(200, 200), 
        new ShaderProgram("/3DObject/modelViewProjection.vert","/Filters/toneMapperLinear.frag"),
        getWidth(window), getHeight(window)))
            ->texture("tex", Texture::load(RESOURCES_PATH "/bambus.jpg"))
            ->update("model", translate(vec3(-0.5,-0.5,0)))
            ->update("projection", perspective(45.0f, getRatio(window), 0.1f, 100.0f))
            ->update("resolution", getResolution(window));

    auto holefilling = (new HierarchicalHoleFilling(
        getResolution(window),
        quad))
            ->texture(sparse->get("fragColor"));

    auto tonemapping = (new RenderPass(
        new Quad(), 
        new ShaderProgram("/Filters/fullscreen.vert","/Filters/toneMapperLinear.frag")))
            ->texture("tex", holefilling->getOutput())
            ->update("resolution", getResolution(window));

    int level = 0;
    bool toggleInOut = true;
    setKeyCallback(window, [&] (int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            switch (key) {
            case GLFW_KEY_PERIOD:
                tonemapping->update("level", (level < holefilling->getMipmapNumber()-1)? ++level : level);
                break;
            case GLFW_KEY_COMMA:
                tonemapping->update("level", (level > 0)? --level : level);
                break;
            case GLFW_KEY_SPACE:
                (toggleInOut ^= 1)? tonemapping->texture("tex", holefilling->getOutput()) : tonemapping->texture("tex", sparse->get("fragColor"));
                break;
            } 
        }
    });

    render(window, [&] (float deltaTime) {
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) (rotY - deltaTime < 0)? rotY -= deltaTime + 6.283 : rotY -= deltaTime;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) (rotY + deltaTime > 6.283)? rotY += deltaTime - 6.283 : rotY += deltaTime;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) (rotX - deltaTime < 0)? rotX -= deltaTime + 6.283 : rotX -= deltaTime;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) (rotX + deltaTime > 6.283)? rotX += deltaTime - 6.283 : rotX += deltaTime;

        sparse
            ->update("view", translate(vec3(0,0,-0.5)) * eulerAngleXY(-rotX, -rotY))
            ->clear()
            ->run();

        holefilling
            ->run();

        tonemapping
            ->clear()
            ->run();
    });
}