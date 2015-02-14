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
    GLFWwindow* window = generateWindow(800, 600);

    AssimpLoader* scene = new AssimpLoader();
    scene->loadDAEFile(RESOURCES_PATH "/obj/cornell-box.obj")
         ->printLog();

    Mesh* mesh0 = scene->getMesh(0);
    Mesh* mesh1 = scene->getMesh(1);
    Mesh* mesh2 = scene->getMesh(2);
    Mesh* mesh3 = scene->getMesh(3);
    auto shaderProgram = new ShaderProgram({"/AssimpLoader/minimal.vert", "/AssimpLoader/minimal.frag"});

    glm::mat4 view       = glm::lookAt(glm::vec3(0, 0, -5), glm::vec3(0,0,0), glm::vec3(0,1,0));
    double aspectRatio   = 800/(double)600;
    glm::mat4 projection = glm::perspective(45.0, aspectRatio, 0.1, 100.0);

    auto pass = new RenderPass(
        mesh0,
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

    float rotateY = 0;
    render(window, [&] (float deltaTime)
    {
        pass->clear(1, 1, 1, 1);

        shaderProgram->update("view", view);
        shaderProgram->update("projection", projection);

        glm::mat4 model = glm::rotate(scene->getModelMatrix(1), rotateY, glm::vec3(0, 1, 0));
        shaderProgram->update("model", model);
        mesh1->draw();
        model = glm::rotate(scene->getModelMatrix(2), rotateY, glm::vec3(0, 1, 0));
        shaderProgram->update("model", model);
        mesh2->draw();
        model = glm::rotate(scene->getModelMatrix(3), rotateY, glm::vec3(0, 1, 0));
        shaderProgram->update("model", model);
        mesh3->draw();


        model = glm::rotate(scene->getModelMatrix(0), rotateY, glm::vec3(0, 1, 0));
        shaderProgram->update("model", model);
        pass->run();

        rotateY += 0.00025f;
    });
}
