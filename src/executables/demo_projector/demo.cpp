#include "ShaderTools/Renderer.h"
#include "ShaderTools/RenderPass.h"
#include "AssetTools/Texture.h"
#include "AssetTools/Mesh.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"

using namespace std;
using namespace glm;

int main(int argc, char *argv[]) {
    GLFWwindow* window = generateWindow();

    float rotX = 0.0f;
    float rotY = 0.0f;
    float turn = 0;
    float speed = 1.0f;
    
    float projectorRotX = 0.0f;
    float projectorRotY = 0.0f;

    mat4 projection = perspective(45.0f, getRatio(window), 0.1f, 100.0f);
    mat4 projectorProjection = perspective(45.0f, getRatio(window), 0.1f, 100.0f);

    auto texture = new Texture(RESOURCES_PATH "/textures/equirectangular/plaza.png");
    auto projectionTexture = new Texture(RESOURCES_PATH "/textures/bambus.jpg");

    RenderPass* sky = (new RenderPass(
        new Quad(), 
        new ShaderProgram("/Filters/fullscreen.vert","/3DObject/equirectangularSky.frag")))
            ->texture("tex", texture->getHandle())
            ->update("resolution", getResolution(window))
            ->update("projection", projection);

    RenderPass* object = (new RenderPass(
        new Mesh(RESOURCES_PATH "/meshes/cv-logo.obj", aiProcess_GenSmoothNormals | aiProcess_Triangulate), 
        new ShaderProgram("/3DObject/modelViewProjection.vert","/3DObject/projector.frag")))
            ->texture("tex", projectionTexture->getHandle())
            ->update("resolution", getResolution(window))
            ->update("projection", projection)
            ->update("projectorProjection", projection)
            ->update("color", vec4(0.9,0.8,0.4,0))
            ->update("blendColor", 0.3f);

    render(window, [&] (float deltaTime) {
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) (rotY - deltaTime < 0)? rotY -= deltaTime + 6.283 : rotY -= deltaTime;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) (rotY + deltaTime > 6.283)? rotY += deltaTime - 6.283 : rotY += deltaTime;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) (rotX - deltaTime < 0)? rotX -= deltaTime + 6.283 : rotX -= deltaTime;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) (rotX + deltaTime > 6.283)? rotX += deltaTime - 6.283 : rotX += deltaTime;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) (rotY - deltaTime < 0)? projectorRotY -= deltaTime + 6.283 : projectorRotY -= deltaTime;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) (projectorRotY + deltaTime > 6.283)? projectorRotY += deltaTime - 6.283 : projectorRotY += deltaTime;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) (projectorRotX - deltaTime < 0)? projectorRotX -= deltaTime + 6.283 : projectorRotX -= deltaTime;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) (projectorRotX + deltaTime > 6.283)? projectorRotX += deltaTime - 6.283 : projectorRotX += deltaTime;

        mat4 view = translate(mat4(1), vec3(0,-7,-30)) * eulerAngleXY(-rotX, -rotY);
        mat4 projectorView = translate(mat4(1), vec3(0,-7,-30)) * eulerAngleXY(-projectorRotX, -projectorRotY);

        sky
            ->clear()
            ->update("view", view)
            ->run();
        object
            ->clearDepth()
            // ->update("model", rotate(mat4(1), (turn + deltaTime > 6.283)? turn += speed * deltaTime * 0.1 - 6.283 : turn += speed * deltaTime * 0.1, vec3(0,1,0)))
            ->update("model", mat4(1))
            ->update("view", view)
            ->update("projectorView", projectorView)
            ->run();
    });
}

