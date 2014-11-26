#include "ShaderTools/DefaultRenderLoop.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include <array>

#include "ShaderTools/VertexArrayObjects/Cube.h"
#include "Compression/TextureTools.h"
#include "Compression/ComputeShaderTools.h"

using namespace std;
using namespace glm;



//TODO draw generic triangle mesh

//TODO indirektionstiefe für 1. Kugel fixen
//TODO 1 farbtextur pro layer
//TODO eliminate background? (for layer)
//TODO 1 positions- / tiefentextur pro layer




auto quadVAO = new Quad();

// basics of fragment shader taken from: https://www.shadertoy.com/view/ldS3DW
// triangle intersection taken from: http://undernones.blogspot.de/2010/12/gpu-ray-tracing-with-glsl.html
// For raytracing
auto sp = new ShaderProgram({"/Raytracing/raytrace.vert", "/Raytracing/raytrace.frag"});
auto pass1 = new RenderPass(
    quadVAO,
    sp, width, height);

//auto sp3 = new ShaderProgram({"/Raytracing/raytrace.vert", "/Raytracing/colorIndirection.frag"});
//auto pass3 = new RenderPass(
//    quadVAO,
//    sp3, width, height);
//
//auto sp4 = new ShaderProgram({"/Raytracing/raytrace.vert", "/Raytracing/depth.frag"});
//auto pass4 = new RenderPass(
//    quadVAO,
//    sp4, width, height);


//For Compression
auto sp2 = new ShaderProgram({"/Compression/test1.vert", "/Compression/test1.frag"});
auto pass2 = new RenderPass(new Cube(), sp2);
GLuint textureHandle = TextureTools::loadTexture(RESOURCES_PATH "/bambus.jpg");
GLuint texHandle = ComputeShaderTools::generateTexture();


//auto pass1 = new RenderPass(
//    new Quad(),
//    sp
//);

auto compSP = new ShaderProgram({"/Raytracing/raytrace.vert", "/Raytracing/compositing.frag"});

auto compositing = new RenderPass(
		quadVAO,
    compSP);


float size = 1.0;
float lum =  0.5;
float rad=0.0;
double xpos, ypos;
int ref=1;

float lastTime, currentTime;

float horizontalAngle=0.0;
float verticalAngle=0.0;

std::vector<glm::vec4> sphereVec;
std::vector<glm::vec3> mesh;
std::vector<glm::vec3> colorSphere;

int main(int argc, char *argv[]) {
    sp -> printUniformInfo();
    sp -> printInputInfo();
    sp -> printOutputInfo();


//    compSP -> printUniformInfo();
//    compSP -> printInputInfo();
//    compSP -> printOutputInfo();

    sphereVec.push_back(glm::vec4(0.0, 0.0, 0.0, 0.5));
    sphereVec.push_back(glm::vec4(0.75, 0.5, 0.5, 0.5));
    sphereVec.push_back(glm::vec4(-0.75, 0.5, 0.5, 0.5));

    //needs to be same size as sphereVec
    colorSphere.push_back(glm::vec3(0.5,0.0,0.0));
    colorSphere.push_back(glm::vec3(0.0,0.5,0.0));
    colorSphere.push_back(glm::vec3(0.0,0.0,0.5));

    mesh.push_back(glm::vec3(-0.5, -0.5, 0.75));
    mesh.push_back(glm::vec3(0.5, -0.5, 0.75));
    mesh.push_back(glm::vec3(0.0, 0.8, 0.75));
//    mesh.push_back(glm::vec3(0.10, 0.11, -0.12));
//    mesh.push_back(glm::vec3(0.13, 0.14, -0.15));
//    mesh.push_back(glm::vec3(0.16, 0.17, -0.18));
//    mesh.push_back(glm::vec3(0.19, 0.20, -0.21));
//    mesh.push_back(glm::vec3(0.22, 0.23, -0.24));
//    mesh.push_back(glm::vec3(0.25, 0.26, -0.27));
//    mesh.push_back(glm::vec3(0.28, 0.29, -0.30));
//    mesh.push_back(glm::vec3(0.31, 0.32, -0.33));
//    mesh.push_back(glm::vec3(0.34, 0.35, -0.36));
//    mesh.push_back(glm::vec3(0.37, 0.38, -0.39));
//    mesh.push_back(glm::vec3(0.40, 0.41, -0.42));
//    mesh.push_back(glm::vec3(0.43, 0.44, -0.45));
//    mesh.push_back(glm::vec3(0.46, 0.47, -0.48));
//    mesh.push_back(glm::vec3(0.49, 0.50, -0.51));
//    mesh.push_back(glm::vec3(0.52, 0.53, -0.54));
//    mesh.push_back(glm::vec3(0.55, 0.56, -0.57));
//    mesh.push_back(glm::vec3(0.58, 0.59, -0.60));
//    mesh.push_back(glm::vec3(0.61, 0.62, -0.63));

    lastTime = glfwGetTime();

    pass1 -> update("sphereVec[0]", sphereVec);
    pass1 -> update("mesh[0]", mesh);
    pass1 -> update("colorSphere[0]", colorSphere);

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
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) rad +=0.0125 * deltaT;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)rad -= 0.0125 * deltaT;
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)ref =1;
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)ref =2;
        if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)ref =0;
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)ref =3;


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
            pass1
            -> clear(0, 0, 0, 0)
            -> update("iGlobalTime", lastTime)
            -> update("iResolution", glm::vec3(width, height, 1))
            -> update("scale", size)
			-> update("zoom", rad)
			-> update("indirection", ref)
            -> update("invViewProjection", invViewProjection)
            -> update("invView",invView)
			-> run();


            compositing
			-> clear(0, 1, 0, 0)
            -> update("scale", size)
	        -> texture("tex1", pass1->get("fragColor"))
			//-> texture("indirection", pass3->get("fragColor"))
			-> run();

        }

    });
}
