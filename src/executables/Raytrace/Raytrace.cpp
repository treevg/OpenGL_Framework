/*
 * main.cpp
 *
 *  Created on: 10.03.2015
 *      Author: Moe
 */
#include <array>
#include <queue>

#include "ShaderTools/Renderer.h"
#include "Compression/TextureTools.h"
#include "ShaderTools/ShaderStorageBuffer.h"
#include "ShaderTools/VertexArrayObjects/Grid.h"
#include "RenderTechniques/HierarchicalHoleFilling.h"

using namespace std;
using namespace glm;

//TODO adjust reflective warping shader
//TODO divide scene into patches for raytracing-> increase performance

int main(int argc, char *argv[]) {

    GLFWwindow* window = generateWindow();

    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    vector<vec4> sphereVec;
    sphereVec.push_back(glm::vec4(0.0, 0.0, 0.0, 0.5));
    sphereVec.push_back(glm::vec4(0.75, 0.5, 0.5, 0.5));
    sphereVec.push_back(glm::vec4(-1.0, 0.5, 1.5, 0.3));

    //needs to be same size as sphereVec
    vector<vec3> colorSphere;
    colorSphere.push_back(glm::vec3(0.8,0.4,0.4));
    colorSphere.push_back(glm::vec3(0.4,0.8,0.4));
    colorSphere.push_back(glm::vec3(0.4,0.4,0.8));

    // latency stuff
    int latencyFrameNumber = 5;
    queue<mat4> latencyQueue;

    auto quadVAO = new Quad();
    auto grid 	 = new Grid(getWidth(window),getHeight(window));


    //Load mesh: parameter is resources path
    auto ssbo2 = new ShaderStorageBuffer("/Objects/plane.obj", false);

    // Raytracing
    // basics of fragment shader taken from: https://www.shadertoy.com/view/ldS3DW
    // triangle intersection taken from: http://undernones.blogspot.de/2010/12/gpu-ray-tracing-with-glsl.html
    auto raytracePass = (new RenderPass(quadVAO,
        new ShaderProgram({"/Raytracing/raytrace.vert", "/Raytracing/raytrace.frag"}),
        getWidth(window), getHeight(window)))
        ->texture("environmentTexture", TextureTools::loadTexture(RESOURCES_PATH "/equirectangular/park.jpg"))
        ->update("sphereVec[0]", sphereVec)
        ->update("colorSphere[0]", colorSphere)
        ->update("colorSphere[0]", colorSphere)
        ->update("iResolution", glm::vec3(getWidth(window), getHeight(window), 1));

    // Compositing
    auto compSP = new ShaderProgram({"/Raytracing/raytrace.vert", "/Raytracing/compositing.frag"});
    auto compositing = new RenderPass(quadVAO, compSP);


    // Diffuse Warping
    auto diffWarp = (new RenderPass(grid,
        new ShaderProgram({"/Raytracing/warp.vert", "/Raytracing/warp.frag"}),
		getWidth(window), getHeight(window)))
        ->update("resolution", getResolution(window))
      //  ->texture("viewDirTexture", raytracePass->get("initialDirNotnorm"))
       // ->texture("colorTexture", raytracePass->get("diffuseColor"))
        // ->texture("diffuseDepthTexture", raytracePass->get("diffuseDepth"))
        ->texture("diffPositionTexture", raytracePass->get("diffusePosition"))
      //  ->texture("indirectColorTexture", raytracePass->get("reflectiveColor"))
        ->texture("normalTexture", raytracePass->get("normal"))
        ->texture("diffuseTexture", raytracePass->get("diffuseColor"));


//    //extra normalwarpPass
//    auto normalWarp = (new RenderPass(grid,
//        new ShaderProgram({"/Raytracing/warp.vert", "/Raytracing/normalWarp.frag"}),
//		getWidth(window), getHeight(window)))
//        ->clear(0,0,0,0)
//        ->texture("normalTexture", raytracePass->get("normal"))
//        ->texture("diffusePositionTexture", raytracePass->get("diffusePosition"));

    // Holefilling
    auto holeFill = (new RenderPass(quadVAO,
    	new ShaderProgram({"/Raytracing/raytrace.vert", "/RenderTechniques/hhfFill.frag"}),
		getWidth(window), getHeight(window)))
		->clear(1,0,0,0)
		->texture("mipmapTexture", diffWarp->get("position"))
		->update("resolution", vec2(getWidth(window), getHeight(window)))
		->update("level", 0);

    // Reflective Warping
    auto refWarp = (new RenderPass(grid,
        new ShaderProgram({"/Raytracing/refWarp.vert", "/Raytracing/refWarp.frag"}),
		getWidth(window), getHeight(window)))
        ->clear(0,0,0,0)
        ->texture("diffusePositionTexture", raytracePass->get("diffusePosition"))
        ->texture("normalTexture", raytracePass->get("normal"))
		->texture("reflectionPositionTexture", raytracePass->get("reflectivePosition"))
		->texture("reflectiveColorTexture", raytracePass->get("reflectiveColor"))
		->update("resolution", getResolution(window));

    // Gather reflection
    auto gatherRefPass = (new RenderPass(quadVAO,
        new ShaderProgram({"/Raytracing/raytrace.vert", "/Raytracing/gatherReflection.frag"}),
        getWidth(window), getHeight(window)))
        ->texture("reflectionColorTexture", raytracePass->get("reflectiveColor"))
        ->texture("reflectionPositionTexture", refWarp->get("position"))
        ->texture("warpedDiffusePositionTexture", holeFill->get("fragColor"))
        ->texture("splattedReflectionUVTexture", refWarp->get("splattedRefUV"))
        ->texture("warpedNormalTexture",  diffWarp->get("normal"))
        ->texture("diffColorTexture", raytracePass->get("diffuseColor"))
        ->update("resolution", getResolution(window))
		;

    auto tonemapping = (new RenderPass(
    new Quad(),
    new ShaderProgram({"/Filters/fullscreen.vert","/Filters/toneMapperLinear.frag"})))
        ->texture("tex", raytracePass->get("diffuseColor"))
        ->update("resolution", getResolution(window));


    setKeyCallback(window, [&] (int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            switch (key) {
            case GLFW_KEY_1:
                tonemapping->texture("tex", raytracePass->get("diffuseDepth"));
                break;
            case GLFW_KEY_2:
                tonemapping->texture("tex", raytracePass->get("diffusePosition"));
                break;
            case GLFW_KEY_3:
                tonemapping->texture("tex", raytracePass->get("reflectiveColor"));
                break;
            case GLFW_KEY_4:
                tonemapping->texture("tex", raytracePass->get("reflectivePosition"));
                break;
            case GLFW_KEY_5:
                tonemapping->texture("tex", raytracePass->get("normal"));
                break;
            case GLFW_KEY_6:
                tonemapping->texture("tex", diffWarp->get("position"));
                break;
            case GLFW_KEY_7:
                tonemapping->texture("tex", diffWarp->get("diffuse"));
                break;
            case GLFW_KEY_8:
                tonemapping->texture("tex", diffWarp->get("normal"));
                break;
            case GLFW_KEY_9:
                 tonemapping->texture("tex", diffWarp->get("uv"));
                 break;
            case GLFW_KEY_0:
                 tonemapping->texture("tex", diffWarp->get("flow"));
                 break;
            case GLFW_KEY_SPACE:
                 tonemapping->texture("tex", refWarp->get("position"));
                 break;
            case GLFW_KEY_Q:
                 tonemapping->texture("tex", refWarp->get("splattedRefUV"));
                 break;
            case GLFW_KEY_W:
                 tonemapping->texture("tex", refWarp->get("refColor"));
                 break;
            case GLFW_KEY_E:
                 tonemapping->texture("tex", gatherRefPass->get("warpedColor"));
                 break;
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            }
        }
    });


    float rotX = 0.0f;
    float rotY = 0.0f;

    render(window, [&] (float deltaTime) {
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) (rotY - deltaTime < 0)? rotY -= deltaTime + 6.283 : rotY -= deltaTime;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) (rotY + deltaTime > 6.283)? rotY += deltaTime - 6.283 : rotY += deltaTime;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) (rotX - deltaTime < 0)? rotX -= deltaTime + 6.283 : rotX -= deltaTime;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) (rotX + deltaTime > 6.283)? rotX += deltaTime - 6.283 : rotX += deltaTime;

        mat4 view = translate(mat4(1), vec3(0,0,-4)) * eulerAngleXY(-rotX, -rotY);

        // Latency simulation
        latencyQueue.push(view);
        while (latencyQueue.size() > latencyFrameNumber) {
            latencyQueue.pop();
        }

        mat4 view_old = latencyQueue.front();
        mat4 invView = inverse(view);
        mat4 invView_old = inverse(view_old);

        mat4 projection = perspective(45.0f, float(getWidth(window))/float(getHeight(window)), 0.001f, 100.0f);
        mat4 invViewProjection = inverse(projection * view);
        mat4 invViewProjection_old = inverse(projection * view_old);
        mat4 vp_old = projection * view_old;


        ssbo2->bind(11);

        raytracePass
        ->clear(0, 0, 0, 0)
        ->update("view", view)
        ->update("projection", projection)
        ->update("invViewProjection", invViewProjection_old)
        ->update("invView",invView_old)
        ->run();

        diffWarp
        ->clear()
        ->update("view", view)
        ->update("projection", projection)
        ->run();

        holeFill
		->clear(1,0,0,0)
		->run();

        refWarp
		->clear(0,1,0,0)
        ->update("altView", view)
        ->update("projection", projection)
		->run();


        gatherRefPass
        ->clear(0,0,1,0)
        ->update("mode" , (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)?1:0)
        ->update("maxGDSteps", 5)
        ->update("mvpOld", vp_old)
        ->update("altView", view)
        ->update("projection", projection)
        ->run();

        tonemapping
        ->clear()
        ->run();

    });
}
