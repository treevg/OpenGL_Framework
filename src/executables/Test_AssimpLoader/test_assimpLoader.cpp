#include "ShaderTools/Renderer.h"
#include "ShaderTools/RenderPass.h"
#include "Compression/TextureTools.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "AssimpLoader/AssimpLoader.h"
#include "AssimpLoader/GenericVertexArrayObject.h"
#include <typeinfo>


using namespace std;
using namespace glm;

int main(int argc, char *argv[]) {
    GLFWwindow* window = generateWindow();

    AssimpLoader* scene = new AssimpLoader();
    scene->loadDAEFile(RESOURCES_PATH "/obj/cube.obj");
    GenericVertexArrayObject* mesh = new GenericVertexArrayObject();

    auto pass = new RenderPass(
        mesh,
        new ShaderProgram({"/AssimpLoader/minimal.vert", "/AssimpLoader/minimal.frag"})
    );

    render(window, [&] (float deltaTime)
    {
        pass
            ->clear(1, 1, 1, 1)
            ->run();
    });
}
