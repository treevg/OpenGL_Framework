#include <vector>
#include "ShaderTools/Renderer.h"
#include "ShaderTools/RenderPass.h"
#include "AssetTools/Texture.h"
#include "AssetTools/CustomZBuffer.h"
#include "AssetTools/Mesh.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "ShaderTools/VertexArrayObjects/ImpostorSpheres.h"

using namespace glm;

float r(float size) {
    return size * 2 * static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - size;
}

float r2(float size) {
    return size * static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

int main(int argc, char *argv[]) {
    GLFWwindow* window = generateWindow();

    float rotX = 0.0f;
    float rotY = 0.0f;
    float distance = 40.0;
    float scale = 1.0;

    mat4 projection = perspective(45.0f, getRatio(window), 0.1f, 100.0f);

    auto texture = new CustomZBuffer(getWidth(window), getHeight(window));
    //auto texture = new Texture(RESOURCES_PATH "/equirectangular/plaza.png");
    
    RenderPass* renderBalls = new RenderPass(
                new ImpostorSpheres(),
                new ShaderProgram("/Impostor/impostorSpheres_Instanced.vert", "/Impostor/impostorSpheres_Instanced.frag")/*,
                getWidth(window),
                getHeight(window)*/);
    renderBalls->texture("tex", texture->getHandle());
    renderBalls->update("projection", projection);

//    RenderPass* output = new RenderPass(
//                new Quad(),
//                new ShaderProgram("/Filters/fullscreen.vert", "/Filters/toneMapperLinear.frag"));
//    output->texture("tex", renderBalls->get("fragColor"));


    glDisable(GL_DEPTH_TEST);
    render(window, [&] (float deltaTime) {
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) (rotY - deltaTime < 0)? rotY -= deltaTime + 6.283 : rotY -= deltaTime;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) (rotY + deltaTime > 6.283)? rotY += deltaTime - 6.283 : rotY += deltaTime;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) (rotX - deltaTime < 0)? rotX -= deltaTime + 6.283 : rotX -= deltaTime;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) (rotX + deltaTime > 6.283)? rotX += deltaTime - 6.283 : rotX += deltaTime;
        if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) distance += deltaTime * 10;
        if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) distance = max(distance - deltaTime * 10, 0.0f);
        if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS) scale += deltaTime;
        if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS) scale = glm::max(scale - deltaTime, 0.01f);

        mat4 view = translate(mat4(1), vec3(0,0,-distance)) * eulerAngleXY(-rotX, -rotY);

        renderBalls->clear(0,0,0,999);
        renderBalls->update("scale", vec2(scale));
        renderBalls->update("view", view);
        texture->clear();
        renderBalls->run();


        //printf("%f, ", deltaTime);

//        output->clear();
//        output->run();
    });
}


