#include "ShaderTools/Renderer.h"
#include "ShaderTools/RenderPass.h"
#include "AssetTools/Texture.h"
#include "AssetTools/Scene.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"

using namespace std;
using namespace glm;

int main(int argc, char *argv[]) {
    GLFWwindow* window = generateWindow();

    float rotX = 0.0f;
    float rotY = 0.0f;
    float distance = 1.0;
    float turn = 0;
    float speed = 1.0f;

    mat4 projection = perspective(45.0f, getRatio(window), 0.1f, 100.0f);

    auto texture = new Texture(RESOURCES_PATH "/textures/equirectangular/plaza.png");

    auto scene = new Scene(RESOURCES_PATH "/meshes/sibenik.obj");

    RenderPass* objects = (new RenderPass(
        new ShaderProgram("/3DObject/modelViewProjection.vert","/3DObject/gBuffer.frag"),
        getWidth(window), getHeight(window)))
            ->texture("tex", texture->getHandle())
            ->update("projection", projection)
            ->update("color", vec4(0.9,0.8,0.4,0))
            ->update("blendColor", 0.3f);

    auto tonemapping = (new RenderPass(
        new Quad(),
        new ShaderProgram("/Filters/fullscreen.vert","/Filters/toneMapperLinear.frag")))
            ->texture("tex", objects->get("color"))
            ->update("minRange", 0.0)
            ->update("maxRange", 1.0);

    setKeyCallback(window, [&] (int key, int scancode, int action, int mods) {
      if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        tonemapping->texture("tex", key  - 49);
      }
    });

    render(window, [&] (float deltaTime) {
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) (rotY - deltaTime < 0)? rotY -= deltaTime + 6.283 : rotY -= deltaTime;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) (rotY + deltaTime > 6.283)? rotY += deltaTime - 6.283 : rotY += deltaTime;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) (rotX - deltaTime < 0)? rotX -= deltaTime + 6.283 : rotX -= deltaTime;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) (rotX + deltaTime > 6.283)? rotX += deltaTime - 6.283 : rotX += deltaTime;
        if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) distance += deltaTime * 10;
        if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) distance = glm::max(distance - deltaTime * 10.0f, 0.0f);
        mat4 view = translate(mat4(1), vec3(0, 0, -distance)) * eulerAngleXY(-rotX, -rotY) * translate(mat4(1), vec3(0, 12, 0));

        objects
            ->clear()
            ->clearDepth();

        for (auto m : scene->meshes) {
            objects
                ->update("model", glm::mat4(1))
                ->update("view", view)
                ->run(m);
        }

        tonemapping
            ->clear()
            ->run();
    });
}
