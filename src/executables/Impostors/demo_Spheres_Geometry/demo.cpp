#include <vector>
#include "ShaderTools/Renderer.h"
#include "ShaderTools/RenderPass.h"
#include "AssetTools/Texture.h"
#include "AssetTools/CustomZBuffer.h"
#include "AssetTools/InstancedMeshExample.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "ShaderTools/VertexArrayObjects/ImpostorSpheres.h"

using namespace glm;



int main(int argc, char *argv[]) {
    GLFWwindow* window = generateWindow();

    float rotX = 0.0f;
    float rotY = 0.0f;
    float distance = 40.0;
    float turn = 0;
    float speed = 1.0f;
    float scale = 1.0f;

    mat4 projection = perspective(45.0f, getRatio(window), 0.1f, 100.0f);

//    auto texture = new Texture(RESOURCES_PATH "/equirectangular/plaza.png");

//    RenderPass* sky = (new RenderPass(
//        new Quad(),
//        new ShaderProgram("/Filters/fullscreen.vert","/3DObject/equirectangularSky.frag")))
//            ->texture("tex", texture->getHandle())
//            ->update("resolution", getResolution(window))
//            ->update("projection", projection);

    InstancedMeshExample *sphere = new InstancedMeshExample(RESOURCES_PATH "/obj/Sphere.obj", aiProcess_GenSmoothNormals | aiProcess_Triangulate);

    RenderPass* renderBalls = (new RenderPass(
        sphere,
        new ShaderProgram("/3DObject/modelViewProjectionInstanced.vert","/Filters/phongStaticLight.frag")))
            //->texture("tex", texture->getHandle())
            ->update("projection", projection);



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

        renderBalls->clear();
        renderBalls
            ->clearDepth()
            ->update("model", mat4(1))        //rotate(mat4(1), (turn + deltaTime > 6.283)? turn += speed * deltaTime * 0.1 - 6.283 : turn += speed * deltaTime * 0.1, vec3(0,1,0)))
            ->update("view", view)
                ->update("scale", scale);
            renderBalls->run();
    });
}
