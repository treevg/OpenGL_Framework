#include <vector>
#include "ShaderTools/Renderer.h"
#include "ShaderTools/RenderPass.h"
#include "AssetTools/Texture.h"
#include "AssetTools/Mesh.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"

using namespace glm;

float r(float size) {
    return size * 2 * static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - size;
}

float r2(float size) {
    return size * static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

int num_balls = 10000;

int main(int argc, char *argv[]) {
    GLFWwindow* window = generateWindow();

    float rotX = 0.0f;
    float rotY = 0.0f;
    float distance = 40.0;
    float scale = 1.0;

    mat4 projection = perspective(45.0f, getRatio(window), 0.1f, 100.0f);
    
    RenderPass* renderBalls = new RenderPass(
        new Quad(),
        new ShaderProgram("/Impostor/impostorSpheres.vert", "/Impostor/impostorSpheres.frag"),
        getWidth(window),
        getHeight(window));
    renderBalls->texture("tex", renderBalls->get("fragColor"));
    renderBalls->update("projection", projection);

    RenderPass* output = new RenderPass(
        new Quad(),
        new ShaderProgram("/Filters/fullscreen.vert", "/Filters/toneMapperLinear.frag"));
    output->texture("tex", renderBalls->get("fragColor"));

    std::vector<vec4> balls;
    for (int i = 0; i < num_balls; i++)
        balls.push_back(vec4(r(15),r(15),r(15),5 + r(2)));

    glDisable(GL_DEPTH_TEST);
    render(window, [&] (float deltaTime) {
        //rotY += deltaTime * 0.1;
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

        for(vec4 b : balls) {
            renderBalls->update("view", view);
            renderBalls->update("position", vec3(b.x, b.y, b.z));
            renderBalls->update("size", vec2(b.w * scale));
            renderBalls->run();
        }

        output->clear();
        output->run();
    });
}


