#include "ShaderTools/DefaultRenderLoop.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include <array>

#include "ShaderTools/VertexArrayObjects/Cube.h"
#include "Compression/TextureTools.h"
#include "Compression/ComputeShaderTools.h"

using namespace std;
using namespace glm;

// fragment shader taken from: https://www.shadertoy.com/view/ldS3DW
auto sp = new ShaderProgram({"/Raytracing/raytrace.vert", "/Raytracing/raytrace2.frag"});
auto sp2 = new ShaderProgram({"/Compression/test1.vert", "/Compression/test1.frag"});

auto pass2 = new RenderPass(new Cube(), sp2);
GLuint textureHandle = TextureTools::loadTexture(RESOURCES_PATH "/bambus.jpg");
GLuint texHandle = ComputeShaderTools::generateTexture();


auto pass = new RenderPass(
    new Quad(), 
    sp
);

//TODO avoid horizonatal line / change background?
//TODO getTextures/depth/color  -> multiple passes?
//TODO raytrace polygons
//TODO fix "depthtest"


float size = 1.0;
float lum =  0.5;
float rad=0.0;
double xpos, ypos;
int ref=1;

float lastTime, currentTime;

float horizontalAngle=4.7;
float verticalAngle=0.0;

std::vector<glm::vec4> sphereVec;
std::vector<glm::vec3> mesh;
std::vector<glm::vec3> colorSphere;

int main(int argc, char *argv[]) {
    sp -> printUniformInfo();
    sp -> printInputInfo();
    sp -> printOutputInfo();


    sphereVec.push_back(glm::vec4(0.0, 0.0, 0.0, 0.5));
    sphereVec.push_back(glm::vec4(0.75, 0.5, 0.5, 0.5));
    sphereVec.push_back(glm::vec4(-0.75, 0.5, 0.5, 0.5));

    //needs to be same size as sphereVec
    colorSphere.push_back(glm::vec3(0.5,0.0,0.0));
    colorSphere.push_back(glm::vec3(0.0,0.5,0.0));
    colorSphere.push_back(glm::vec3(0.0,0.0,0.5));

    mesh.push_back(glm::vec3(0.1, 0.2, -0.3));
    mesh.push_back(glm::vec3(0.4, 0.5, -0.6));
    mesh.push_back(glm::vec3(0.7, 0.8, -0.9));
    mesh.push_back(glm::vec3(0.10, 0.11, -0.12));
    mesh.push_back(glm::vec3(0.13, 0.14, -0.15));
    mesh.push_back(glm::vec3(0.16, 0.17, -0.18));
    mesh.push_back(glm::vec3(0.19, 0.20, -0.21));
    mesh.push_back(glm::vec3(0.22, 0.23, -0.24));
    mesh.push_back(glm::vec3(0.25, 0.26, -0.27));
    mesh.push_back(glm::vec3(0.28, 0.29, -0.30));
    mesh.push_back(glm::vec3(0.31, 0.32, -0.33));
    mesh.push_back(glm::vec3(0.34, 0.35, -0.36));
    mesh.push_back(glm::vec3(0.37, 0.38, -0.39));
    mesh.push_back(glm::vec3(0.40, 0.41, -0.42));
    mesh.push_back(glm::vec3(0.43, 0.44, -0.45));
    mesh.push_back(glm::vec3(0.46, 0.47, -0.48));
    mesh.push_back(glm::vec3(0.49, 0.50, -0.51));
    mesh.push_back(glm::vec3(0.52, 0.53, -0.54));
    mesh.push_back(glm::vec3(0.55, 0.56, -0.57));
    mesh.push_back(glm::vec3(0.58, 0.59, -0.60));
    mesh.push_back(glm::vec3(0.61, 0.62, -0.63));

    lastTime = glfwGetTime();

    pass -> update("sphereVec[0]", sphereVec);
    pass -> update("mesh[0]", mesh);
    pass -> update("colorSphere[0]", colorSphere);

    renderLoop([]{
        currentTime = glfwGetTime();
        float deltaT = currentTime - lastTime;
        lastTime = currentTime;

        glfwGetCursorPos(window, &xpos,&ypos);
        glfwSetCursorPos(window, float(width)/2, float(height)/2);
        horizontalAngle += 0.05 * deltaT * float(float(width)/2 - xpos );
        verticalAngle   += 0.05 * deltaT * float( float(height)/2 - ypos );


        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) size  = glm::max(size - 0.5 * deltaT, 0.);
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) size = glm::min(size + 0.5 * deltaT, 1.);
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) lum  = glm::max(lum - 0.5 * deltaT, 0.);
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) lum = glm::min(lum + 0.5 * deltaT, 1.);
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) rad +=0.005 * deltaT;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)rad -= 0.005 * deltaT;
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)ref =1;
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)ref =2;
        if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)ref =0;


        mat4 view(1);
        view = translate(view, vec3(0,0,-4));
        view = rotate(view, -verticalAngle, vec3(1,0,0));
        view = rotate(view, -horizontalAngle, vec3(0,1,0));

        mat4 invView = inverse(view);

        vec4 pos = invView * vec4(0,0,0,1);
        vec4 dir = normalize(invView * vec4(0,0,1,0));


        mat4 projection = perspective(45.0f, float(width)/float(height), 0.1f, 100.0f);
        mat4 invViewProjection = inverse(projection * view);
        // cout << to_string(dir) << endl;

        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
            pass2
            -> clear(1, 1, 1, 0)
            -> update("uniformView", view)
            -> update("uniformProjection", glm::perspective(45.0f, float(width)/float(height), 0.1f, 100.0f))
            -> update("uniformModel", mat4(1))
            -> texture("tex2", textureHandle)
            -> run();
        } else {
            pass
            -> clear(0, 0, 0, 0)
            -> update("iGlobalTime", lastTime)
            -> update("iResolution", glm::vec3(width, height, 1))
            -> update("scale", size)
			-> update("zoom", rad)
			-> update("indirection", ref)
            -> update("invViewProjection", invViewProjection)
            -> update("invView",invView)
			-> run();
        }

    });
}
