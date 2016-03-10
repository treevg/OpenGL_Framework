#include "ShaderTools/Renderer.h"
#include "ShaderTools/RenderPass.h"
#include "AssetTools/Texture.h"
#include "AssetTools/Scene.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"

using namespace std;
using namespace glm;

int main(int argc, char *argv[]) {
    GLFWwindow* window = generateWindow();

    float rotX = 3.142f;
    float rotY = 0.0f;
    float distance = 0.0;

    mat4 projection = perspective(0.4f * 3.142f, getRatio(window), 0.1f, 100.0f);

    auto skyTex = new Texture(RESOURCES_PATH "/textures/equirectangular/park.jpg");
    auto scene = new Scene(RESOURCES_PATH "/meshes/sibenik.obj");

    auto objects = (new RenderPass(
        new ShaderProgram("/3DObject/modelViewProjection.vert","/3DObject/gBuffer.frag"),
        getWidth(window), getHeight(window)))
            ->texture("tex", skyTex->getHandle())
            ->update("projection", projection);

    auto sky = (new RenderPass(
        new Quad(),
        new ShaderProgram("/Filters/fullscreen.vert","/3DObject/equirectangularSky.frag"),
        objects->frameBufferObject))
            ->texture("tex", skyTex->getHandle())
            ->update("resolution", getResolution(window))
            ->update("projection", projection);

    auto unwrap = (new RenderPass(
        new Quad(),
        new ShaderProgram("/Filters/fullscreen.vert","/Reprojection/equirectangularUnwrap.frag"),
        getWidth(window), getHeight(window)))
            ->texture("tex", objects->get("position"))
            ->update("resolution", getResolution(window))
            ->update("projection", projection)
            ->clear();

    auto tonemapping = (new RenderPass(
        new Quad(),
        new ShaderProgram("/Filters/fullscreen.vert","/Filters/toneMapperLinear.frag")))
        ->texture("tex", unwrap->get("fragColor"))
        ->update("minRange", -20.0)
        ->update("maxRange", 20.0);


    setKeyCallback(window, [&] (int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            if (key == GLFW_KEY_1) tonemapping->texture("tex", unwrap->get("fragColor"));
            if (key == GLFW_KEY_2) tonemapping->texture("tex", objects->get("position"));
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

        sky
        ->clear()
        ->update("view", view)
        ->run()
        ->clearDepth();

        for (auto m : scene->meshes) {
            objects
            ->update("model", glm::mat4(1))
            ->update("view", view)
            ->run(m);
        }

        unwrap
        ->clearDepth()
        ->update("view", view)
        ->run();

        tonemapping
            ->clear()
            ->run();
    });
}
