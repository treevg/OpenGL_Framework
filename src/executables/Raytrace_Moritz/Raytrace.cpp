#include "ShaderTools/DefaultRenderLoop.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"

auto sp = new ShaderProgram({"/Test_ShaderTools/Moritz_Ba/raytrace.vert", "/Test_ShaderTools/Moritz_Ba/raytrace.frag"});

auto pass = new RenderPass(
    new Quad(), 
    sp
);

float size = 0.5;
float lum = 0.5;
glm::vec4 sphere1 = glm::vec4(0.0, 0.0, 0.0, 0.25);  // vec4(.x, .y, .z, rad)

float lastTime, currentTime;

int main(int argc, char *argv[]) {
    sp -> printUniformInfo();
    sp -> printInputInfo();
    sp -> printOutputInfo();

    lastTime = glfwGetTime();

    renderLoop([]{
        currentTime = glfwGetTime();
        float deltaT = currentTime - lastTime;
        lastTime = currentTime;

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) size  = glm::max(size - 0.5 * deltaT, 0.);
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) size = glm::min(size + 0.5 * deltaT, 1.);
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) lum  = glm::max(lum - 0.5 * deltaT, 0.);
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) lum = glm::min(lum + 0.5 * deltaT, 1.);

        pass
        -> clear(0, 0, 0, 0)
        -> update("color", glm::vec4(1,0,0,1))
        -> update("sphere1", sphere1)
		-> update("nicht_gesetzte_uniform", glm::vec4(1, 3, 3, 7))
        -> update("scale", size)
        -> update("luminance", lum)
        -> run();
    });
}
