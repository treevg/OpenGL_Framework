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
    sphereVec.push_back(glm::vec4(-1.0, 0.5, 2.5, 0.3));

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
        ->update("iResolution", glm::vec3(getWidth(window), getHeight(window), 1));

    // Compositing
    auto compSP = new ShaderProgram({"/Raytracing/raytrace.vert", "/Raytracing/compositing.frag"});
    auto compositing = new RenderPass(quadVAO, compSP);



    // Warping
    auto diffWarp = (new RenderPass(grid,
        new ShaderProgram({"/Raytracing/warp.vert", "/Raytracing/warp.frag"}),
        getWidth(window), getHeight(window)))
        ->clear(0,0,0,0)
        ->texture("viewDirTexture", raytracePass->get("initialDirNotnorm"))
        ->texture("colorTexture", raytracePass->get("diffuseColor"))
        ->texture("depthTexture", raytracePass->get("diffuseDepth"))
        ->texture("diffPositionTexture", raytracePass->get("diffusePosition"))
        ->texture("indirectColorTexture", raytracePass->get("fragColor2"))
        ->texture("normalTexture", raytracePass->get("normal"))
        ->texture("depth2Texture", raytracePass->get("reflectiveDepth"));

    // Reflective Warping
    auto refWarp = (new RenderPass(grid,
        new ShaderProgram({"/Raytracing/refWarp.vert", "/Raytracing/refWarp.frag"}),
		getWidth(window), getHeight(window)))
        ->clear(0,0,0,0)
        ->texture("diffusePositionTexture", raytracePass->get("diffusePosition"))
        ->texture("normalTexture", raytracePass->get("normal"))
		->texture("reflectionPositionTexture", raytracePass->get("reflectivePosition"))
        ->texture("reflectionColorTexture", raytracePass->get("reflectiveColor"));

    // Gather reflection
    auto gatherRefPass = (new RenderPass(quadVAO,
        new ShaderProgram({"/Raytracing/raytrace.vert", "/Raytracing/gatherReflection.frag"}),
        getWidth(window), getHeight(window)))
        ->texture("reflectionColorTexture", raytracePass->get("reflectiveColor"))
        ->texture("reflectionPositionTexture", diffWarp->get("refPos"))
        ->texture("warpedDiffusePositionTexture", diffWarp->get("warpDiffPos"))
        ->texture("splattedReflectionUVTexture", diffWarp->get("coColor"))
        ->texture("warpedNormalTexture", diffWarp->get("warpNormal"))
        ->texture("diffColorTexture", diffWarp->get("diffCol"))
        ->texture("eyeNewDirTexture",diffWarp->get("newViewDirection"));

    auto tonemapping = (new RenderPass(
    new Quad(),
    new ShaderProgram({"/Filters/fullscreen.vert","/Filters/toneMapperLinear.frag"})))
        ->texture("tex", raytracePass->get("diffuseColor"))
        ->update("resolution", getResolution(window));


    setKeyCallback(window, [&] (int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            switch (key) {
            case GLFW_KEY_1:
                tonemapping->texture("tex", raytracePass->get("diffuseColor"));
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
                tonemapping->texture("tex", diffWarp->get("diffCol"));
                break;
            case GLFW_KEY_7:
                tonemapping->texture("tex", diffWarp->get("warpDiffPos"));
                break;
//            case GLFW_KEY_8:
//                tonemapping->texture("tex", diffWarp->get("warpNormal"));
//                break;
            case GLFW_KEY_8:
                tonemapping->texture("tex", refWarp->get("splattedRefUV"));
                break;
            case GLFW_KEY_9:
                 tonemapping->texture("tex", refWarp->get("warpedReflectivePosition"));
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

        mat4 projection = perspective(45.0f, float(getWidth(window))/float(getHeight(window)), 0.1f, 100.0f);
        mat4 invViewProjection = inverse(projection * view);
        mat4 invViewProjection_old = inverse(projection * view_old);
        mat4 vp_old = projection * view_old;


        ssbo2->bind(11);

        raytracePass
        ->clear(0, 0, 0, 0)
        ->update("invViewProjection", invViewProjection_old)
        ->update("invView",invView_old)
        ->run();

        diffWarp
        ->clear(0,0,0,0)
        ->update("warpOnOff", (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)?1:0)
        ->update("altView", view)
        ->update("invViewProjection", invViewProjection_old)
        ->update("projection", projection)
        ->run();

        refWarp
		->clear(0,1,0,0)
        ->update("altView", view)
        ->update("projection", projection)
		->run();


        gatherRefPass
        ->clear(0,0,1,0)
        ->update("resolution", getResolution(window))
        ->update("mode" , (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)?1:0)
        ->update("maxGDSteps", 10)
        ->update("mvpOld", vp_old)
        ->run();

        tonemapping
        ->clear()
        ->run();
    });
}
