#include "ShaderTools/Renderer.h"
#include "ShaderTools/RenderPass.h"
#include "Compression/TextureTools.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "AssimpLoader/AssimpLoader.h"
#include "AssimpLoader/Mesh.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"
#include <typeinfo>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

int main(int argc, char *argv[]) {
    GLFWwindow* window = generateWindow();

    AssimpLoader* scene = new AssimpLoader();
    scene->loadDAEFile(RESOURCES_PATH "/obj/cv-logo.obj");
    Mesh* mesh = scene->getMesh();
    auto shaderProgram = new ShaderProgram({"/AssimpLoader/minimal.vert", "/AssimpLoader/minimal.frag"});

    glm::mat4 model      = scene->getModelMatrix();
    glm::mat4 view       = glm::lookAt(glm::vec3(0, 0, -1), glm::vec3(0,0,0), glm::vec3(0,1,0));
    glm::mat4 projection = glm::perspective(90.0, 1.0, 0.1, 100.0);

    auto pass = new RenderPass(
        mesh,
        shaderProgram
    );

    setKeyCallback(window, [&](int key, int scancode, int action, int mode)
    {
        float speedX = 0.0;
        float speedY = 0.0;
        float speedZ = 0.0;

        if(key == GLFW_KEY_W)
            speedZ -= 0.1;
        if(key == GLFW_KEY_A)
            speedX -= 0.1;
        if(key == GLFW_KEY_S)
            speedZ += 0.1;
        if(key == GLFW_KEY_D)
            speedX += 0.1;
        if(key == GLFW_KEY_E)
            speedY += 0.1;
        if(key == GLFW_KEY_Q)
            speedY -= 0.1;

        view = glm::translate(view, glm::vec3(speedX, speedY, speedZ));
    });

    render(window, [&] (float deltaTime)
    {
        pass->clear(1, 1, 1, 1);

        shaderProgram->update("model", model);
        shaderProgram->update("view", view);
        shaderProgram->update("projection", projection);

        pass->run();
    });
}
