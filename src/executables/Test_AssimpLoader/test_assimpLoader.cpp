#include "ShaderTools/Renderer.h"
#include "ShaderTools/RenderPass.h"
#include "Compression/TextureTools.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "AssimpLoader/AssimpLoader.h"
#include "AssimpLoader/Mesh.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"
#include <typeinfo>
#include <glm/gtc/matrix_transform.hpp>
#include <Compression/NewRenderLoop.h>
#include <ShaderTools/DefaultRenderLoop.h>
#include <caca_conio.h>

using namespace std;
using namespace glm;

int main(int argc, char *argv[]) {
    GLFWwindow* window = generateWindow(800, 600);

    AssimpLoader* scene = new AssimpLoader();
    scene->loadDAEFile(RESOURCES_PATH "/obj/cornell-box.obj")
         ->printLog();

    std::vector<Mesh*>* _meshes = scene->getMeshList();
    auto shaderProgram = new ShaderProgram({"/AssimpLoader/minimal.vert", "/AssimpLoader/minimal.frag"});

    glm::mat4 view       = glm::lookAt(glm::vec3(0, 1.5, -2.5), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    double aspectRatio   = getWidth(window)/(double)getHeight(window);
    glm::mat4 projection = glm::perspective(45.0, aspectRatio, 0.1, 100.0);

    auto pass = new RenderPass(
        _meshes->at(0),
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
            speedY -= 0.1;
        if(key == GLFW_KEY_Q)
            speedY += 0.1;

        view = glm::translate(view, glm::vec3(speedX, speedY, speedZ));
    });

    setMouseButtonCallback(window, [&](int button, int action, int modKey){
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        std::cout << mouseX << ", " << mouseY << std::endl;
        if(action == GLFW_PRESS)
            if(button == GLFW_MOUSE_BUTTON_1)
                std::cout << "Mouse 1 was pressed" << std::endl;
            if(button == GLFW_MOUSE_BUTTON_2)
                std::cout << "Mouse 2 was pressed" << std::endl;
            if(button == GLFW_MOUSE_BUTTON_3)
                std::cout << "Mouse 3 was pressed" << std::endl;
    });

    setCursorPosCallback(window, [&](double mouseX, double mouseY){
        float angleX = 0.0;
        float angleY = 0.0;
        float angleZ = 0.0;
        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
        {
            if(mouseX < getWidth(window)/2)
                angleX = -0.1f;
            if(mouseX > getWidth(window)/2)
                angleX =  0.1f;
        }

        view = glm::rotate(view, angleX, glm::vec3(view[1].x, view[1].y, view[1].z));
        //view = glm::rotate(view, angleY, glm::vec3(view[1].x, view[1].y, view[1].z));
        //view = glm::rotate(view, angleZ, glm::vec3(view[2].x, view[2].y, view[2].z));
    });

    float rotateY = 0;
    render(window, [&] (float deltaTime)
    {
        pass->clear(0, 0, 0, 1);

        shaderProgram->update("view", view);
        shaderProgram->update("projection", projection);

        for(unsigned int m = 1; m < _meshes->size(); ++m)
        {
            glm::mat4 model = glm::rotate(scene->getModelMatrix(m), rotateY, glm::vec3(0, 1, 0));
            shaderProgram->update("model", model);
            shaderProgram->update("materialColor", scene->getMaterialColor(_meshes->at(m)->getMaterialIndex()));
            _meshes->at(m)->draw();
        }

        glm::mat4 model = glm::rotate(scene->getModelMatrix(0), rotateY, glm::vec3(0, 1, 0));
        shaderProgram->update("model", model);
        shaderProgram->update("materialColor", scene->getMaterialColor(_meshes->at(0)->getMaterialIndex()));
        pass->run();

        //rotateY += 0.0005f;
    });
}
