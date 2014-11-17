#include "ShaderTools/DefaultRenderLoop.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"
#include "Compression/TextureTools.h"
#include "Compression/ComputeShaderTools.h"

using namespace std;
using namespace glm;

auto sp = new ShaderProgram({"/Compression/test1.vert", "/Compression/test1.frag"});
auto pass = new RenderPass(new Cube(), sp, width, height);
//auto pass = new RenderPass(new Cube(), sp);

auto compositingSP = new ShaderProgram({"/Compression/pass.vert", "/Compression/compositing.frag"});

auto pass2 = new RenderPass(new Quad(), compositingSP);

auto cs = new ShaderProgram(GL_COMPUTE_SHADER, "/Compression/compute.comp");

float cubeAngle = 0.0f;
float rotationSpeed = 0.01f;

glm::mat4 viewMat       = glm::lookAt(
    glm::vec3(0,10,10), // Camera is at (0,10,10), in World Space
    glm::vec3(0,0,0), // and looks at the origin
    glm::vec3(0,1,0));  // Head is up (set to 0, 1 ,0 to look upside-down)

using namespace glm;

glm::mat4 projMat = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
mat4 cubeModel = translate(mat4(1.0f), vec3(0.0f, 1.0f, 0.0f));

GLuint textureHandle = TextureTools::loadTexture(RESOURCES_PATH "/cubeTexture.jpg");
GLuint tex1Handle = ComputeShaderTools::generateTexture();

void invertImg(){
	//cs->texture("outImg", tex1Handle);
	cs->use();
	glBindImageTexture(0, pass->frameBufferObject->getColorAttachment(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	glBindImageTexture(1, tex1Handle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	cs->texture("outImg", tex1Handle);
	cs->texture("inImg", pass->frameBufferObject->getColorAttachment());
	glDispatchCompute(1280/32, 720/32, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

int main(int argc, char *argv[]) {
    sp -> printUniformInfo();
    sp -> printInputInfo();
    sp -> printOutputInfo();

    compositingSP->printUniformInfo();
    compositingSP->printInputInfo();
    compositingSP->printOutputInfo();

    cs->printUniformInfo();
    cs->printOutputInfo();

    renderLoop([]{
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
        -> run();

        //cs->texture("inImg", pass->frameBufferObject->getColorAttachment());		//update input image for compute shader

        invertImg();

        pass2
        ->clear(1, 1, 1, 0)
        ->texture("tex2", tex1Handle)
//        ->texture("tex2", pass->frameBufferObject->getColorAttachment())
        ->run();


    });
}
