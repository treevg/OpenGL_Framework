#include "ShaderTools/Renderer.h"
#include "ShaderTools/RenderPass.h"
#include "Compression/TextureTools.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include <typeinfo>

using namespace std;
using namespace glm;

int main(int argc, char *argv[]) {
    GLFWwindow* window = generateWindow();
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    int texHandle = TextureTools::loadTexture(RESOURCES_PATH "/equirectangular/park.jpg");

    RenderPass* renderSky = new RenderPass(
        new Quad(), 
        new ShaderProgram({"/Filters/fullscreen.vert","/3DObject/equirectangularSky.frag"})
        );

    float rotY = 0;
    float rotX = 0;
    render(window, [&] (float deltaTime) {
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) (rotY - deltaTime < 0)? rotY -= deltaTime + 6.283 : rotY -= deltaTime;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) (rotY + deltaTime > 6.283)? rotY += deltaTime - 6.283 : rotY += deltaTime;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) (rotX - deltaTime < 0)? rotX -= deltaTime + 6.283 : rotX -= deltaTime;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) (rotX + deltaTime > 6.283)? rotX += deltaTime - 6.283 : rotX += deltaTime;

        renderSky
            ->clear(0,0,0,0)
            ->texture("tex", texHandle)
            ->update("view", eulerAngleXY(rotX, rotY))
            ->update("projection", perspective(45.0f, getRatio(window), 0.1f, 100.0f))
            ->run();
    });
}


