#include "ShaderTools/Renderer.h"
#include "ShaderTools/RenderPass.h"
#include "Compression/TextureTools.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include <typeinfo>
#include <chrono>
#include <thread>

using namespace std;
using namespace glm;

int main(int argc, char *argv[]) {
    GLFWwindow* window = generateWindow();
    cout << getWidth(window) << " " << getWidth(window) << endl;
    
    cout << getWidth(window) << " " << getHeight(window) << endl;
    RenderPass* raytrace = (new RenderPass(
        new Quad(), 
        new ShaderProgram("/Filters/fullscreen.vert","/Raytracing/raytrace2.frag")
    ))
        ->update("sphereVec[0]", vector<vec4>{
            vec4(0.0, 0.0, 0.0, 0.5),
            vec4(0.75, 0.5, 0.5, 0.5),
            vec4(-0.75, 0.5, 0.5, 0.5)})
        ->update("colorSphere[0]", vector<vec3>{
            vec3(0.8,0.4,0.4),
            vec3(0.4,0.8,0.4),
            vec3(0.4,0.4,0.8)})
        // ->update("mesh[0]", mesh)
        // ->update("colorTriangle[0]", colorTriangle)
        // ->update("mesh[0]", objl->vertices)
        // ->update("meshData", sp->meshData)
        ->update("iResolution", vec3(1024.0f, 768.0f, 1));

    mat4 view = translate(mat4(1), vec3(0,0,-4));
    // view = rotate(view, -verticalAngle, vec3(1,0,0));
    // view = rotate(view, -horizontalAngle, vec3(0,1,0));
    mat4 projection = perspective(45.0f, (float)1024.0f/768.0f, 0.1f, 100.0f);

    float rad = 0;
    mat4 invView = inverse(view);
    mat4 invProjection = inverse(projection);
    mat4 invViewProjection = inverse(projection * view);

    // setKeyCallback(window, [&] (int key, int scancode, int action, int mods) {
    //     if (action == GLFW_PRESS || action == GLFW_REPEAT) {        
    //         if (key == GLFW_KEY_1) { 
    //         }
    //         if (key == GLFW_KEY_2) { 
    //         }
    //     }
    // });

    render(window, [&] (float deltaTime) {
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) rad += 0.03 * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) rad -= 0.03 * deltaTime;
        raytrace
            -> clear(0, 0, 0, 0)
            -> update("zoom", rad)
            -> update("invViewProjection", invViewProjection)
            -> update("invView", invView)
            -> run();
    });
}