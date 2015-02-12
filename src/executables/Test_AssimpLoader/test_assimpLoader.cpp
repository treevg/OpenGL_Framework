#include "ShaderTools/Renderer.h"
#include "ShaderTools/RenderPass.h"
#include "Compression/TextureTools.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "AssimpLoader/AssimpLoader.h"
#include "AssimpLoader/GenericVertexArrayObject.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"
#include <typeinfo>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

int main(int argc, char *argv[]) {
    GLFWwindow* window = generateWindow();

    AssimpLoader* scene = new AssimpLoader();
    scene->loadDAEFile(RESOURCES_PATH "/obj/cube.obj");
    GLuint vao = scene->getVAO(0);
    GenericVertexArrayObject* mymesh = new GenericVertexArrayObject(vao);
    auto cube = new Cube();
    auto shaderProgram = new ShaderProgram({"/AssimpLoader/minimal.vert", "/AssimpLoader/minimal.frag"});

    glm::mat4 model      = glm::mat4(1.0);
    glm::mat4 view       = glm::lookAt(glm::vec3(2,0,-2), glm::vec3(0,0,0), glm::vec3(0,1,0));
    glm::mat4 projection = glm::perspective(90.0, 1.0, 0.1, 100.0);

    auto pass = new RenderPass(
        cube,
        shaderProgram
    );

    setKeyCallback(window, [&](int key, int scancode, int action, int mode)
    {
        if(key == GLFW_KEY_W)
            view = glm::translate(view, glm::vec3( 0.0,  0.0, -0.1));
        if(key == GLFW_KEY_A)
            view = glm::translate(view, glm::vec3(-0.1,  0.0,  0.0));
        if(key == GLFW_KEY_S)
            view = glm::translate(view, glm::vec3( 0.1,  0.0,  0.0));
        if(key == GLFW_KEY_D)
            view = glm::translate(view, glm::vec3( 0.0,  0.0,  0.1));
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
