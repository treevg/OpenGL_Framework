#include "ShaderTools/Sebastian_Tools/NewRenderLoop.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"
#include "ShaderTools/Sebastian_Tools/TextureTools.h"

auto quadVAO = new Quad();

auto sp = new ShaderProgram({"/Test_ShaderTools/Sebastian_Ba/test1.vert", "/Test_ShaderTools/Sebastian_Ba/test1.frag"});
//auto simpleTexShader = new ShaderProgram({"Test_ShaderTools/test.vert", "/Test_ShaderTools/Sebastian/simpleTexture.frag"});

GLuint shader, program;
static const GLchar* source[] =
{
"#version 430 core\n"
"\n"
"// Input layout qualifier declaring a 16 x 16 (x 1) local\n"
"// workgroup size\n"
"layout (local_size_x = 16, local_size_y = 16) in;\n"
"\n"
"void main(void)\n"
"{\n"
" // Do nothing.\n"
"}\n"
};

auto cs = new ShaderProgram(GL_COMPUTE_SHADER, "/Test_ShaderTools/Sebastian_Ba/compute.comp");

auto pass = new RenderPass(new Cube(), sp);
auto passCompute = new RenderPass(quadVAO, cs, width, height);

float size = 0.5;    			//not needed atm
float lum = 0.5;

float cubeAngle = 0.0f;
float rotationSpeed = 0.01f;

glm::mat4 viewMat       = glm::lookAt(
    glm::vec3(0,10,10), // Camera is at (0,10,10), in World Space
    glm::vec3(0,0,0), // and looks at the origin
    glm::vec3(0,1,0));  // Head is up (set to 0, 1 ,0 to look upside-down)

using namespace glm;

glm::mat4 projMat = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
mat4 cubeModel = translate(mat4(1.0f), vec3(0.0f, 1.0f, 0.0f));

GLuint textureHandle = TextureTools::loadTexture("/libraries/ShaderTools/Sebastian_Tools/cubeTexture.jpg");




int main(int argc, char *argv[]) {
    sp -> printUniformInfo();
    sp -> printInputInfo();
    sp -> printOutputInfo();

    renderLoop([]{
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) size  = glm::max(size - 0.001, 0.);
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) size = glm::min(size + 0.001, 1.);
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) lum  = glm::max(lum - 0.001, 0.);
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) lum = glm::min(lum + 0.001, 1.);
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {glfwDestroyWindow(window); exit(-1);};						//close the window

        //rotate and translate the cube for a neat little animation
        cubeAngle = fmod((cubeAngle + rotationSpeed * glfwGetTime()), (pi<float>() * 2.0f));
        glfwSetTime(0.0);
        cubeModel = translate(rotate(mat4(1.0f), degrees(cubeAngle), vec3(1.0f, 1.0f, 0.0f)), vec3(0.0f, 2.0f, -2.0f));

        pass
        -> clear(1, 1, 1, 0)
        -> update("uniformView", viewMat)
        -> update("uniformProjection", projMat)
        -> update("uniformModel", cubeModel)
        -> texture("tex2", textureHandle)
        -> update("color", glm::vec4(1,0,0,1))
        -> update("luminance", lum)
        -> run();
    });
}
