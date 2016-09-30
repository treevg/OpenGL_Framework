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

//TODO blur compositing?
//TODO smooth normals
//TODO divide scene into patches for raytracing-> increase performance

// usage:
// b: raytrace
// c,v: gd-steps
// x: original viw
// z: novel view


int main(int argc, char *argv[]) {

    GLFWwindow* window = generateWindow(1024, 1024);

    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    vector<vec4> sphereVec;
    sphereVec.push_back(glm::vec4(0.0, 0.0, 0.0, 0.1));
    sphereVec.push_back(glm::vec4(-0.5, 0.5, 1.0, 0.4));

    //needs to be same size as sphereVec
    vector<vec4> colorSphere;
    colorSphere.push_back(glm::vec4(1.0,0.4,0.4,0.9));
    colorSphere.push_back(glm::vec4(0.4,1.0,0.4,0.2));
   // colorSphere.push_back(glm::vec3(0.4,0.4,0.8));

    //normal adjustment
    float factor = 0.5;
    float zoom = -24.0;

    int gradientDescentSteps = 20;

    // latency stuff
    int latencyFrameNumber = 2;
    queue<mat4> latencyQueue;

    auto quadVAO = new Quad();
    auto grid 	 = new Grid(getWidth(window),getHeight(window));


    //Load mesh: parameter is resources path
    //auto ssbo2 = new ShaderStorageBuffer("/Objects/smallBunnyScene.obj", false);
    //auto ssbo2 = new ShaderStorageBuffer("/Objects/twoplanes.obj", false);
    auto ssbo2 = new ShaderStorageBuffer("/Objects/bunny1k.obj", false);

    // Normal Raytracing
    auto raytracePassNormal = (new RenderPass(quadVAO,
        new ShaderProgram({"/Raytracing/raytrace.vert", "/Raytracing/raytraceNormal.frag"}),
        getWidth(window), getHeight(window)))
        //->update("sphereVec[0]", sphereVec)
        ->update("resolution", glm::vec3(getWidth(window), getHeight(window), 1));

    // Normal Raytracing
    auto smoothNormal = (new RenderPass(quadVAO,
        new ShaderProgram({"/Filters/fullscreen.vert", "/Filters/boxBlur.frag"}),
        getWidth(window), getHeight(window)))
        //->update("sphereVec[0]", sphereVec)
		->texture("tex", raytracePassNormal->get("normal"))
		->update("strength", 10)
        ->update("resolution", glm::vec3(getWidth(window), getHeight(window), 1));

    // Raytracing
    auto raytracePass = (new RenderPass(quadVAO,
        new ShaderProgram({"/Raytracing/raytrace.vert", "/Raytracing/raytrace.frag"}),
        getWidth(window), getHeight(window)))
        ->texture("environmentTexture", TextureTools::loadTexture(RESOURCES_PATH "/equirectangular/park.jpg"))
        //->texture("normalTexture", raytracePassNormal->get("normal"))
		->texture("normalTexture", smoothNormal->get("fragColor"))
		//->update("sphereVec[0]", sphereVec)
        //->update("colorSphere[0]", colorSphere)
        ->update("resolution", glm::vec3(getWidth(window), getHeight(window), 1));
    ssbo2->bind(11);


    // Diffuse Warping
    auto diffWarp = (new RenderPass(grid,
        new ShaderProgram({"/Raytracing/warp.vert", "/Raytracing/warp.frag"}),
		getWidth(window), getHeight(window)))
        ->update("resolution", getResolution(window))
        ->texture("diffPositionTexture", raytracePass->get("diffusePosition"))
        ->texture("reflectionPositionTexture", raytracePass->get("reflectivePosition"))
        ->texture("normalTexture", raytracePass->get("normal"))
        ->texture("diffuseTexture", raytracePass->get("diffuseColor"));

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
		->texture("refPositionTexture", raytracePass->get("reflectivePosition"))
		->texture("reflectiveColorTexture", raytracePass->get("reflectiveColor"))
		->update("resolution", getResolution(window));

    // Gather reflection
    auto gatherRefPass = (new RenderPass(quadVAO,
        new ShaderProgram({"/Raytracing/raytrace.vert", "/Raytracing/gather.frag"}),
        getWidth(window), getHeight(window)))
        ->texture("uvDiffuse", diffWarp->get("uv"))
        ->texture("uvReflect", refWarp->get("uv"))
        ->texture("colorReflect", raytracePass->get("reflectiveColor"))
		->texture("warpedDiffusePositionTexture", diffWarp->get("position"))
		->texture("warpedNormalTexture", diffWarp->get("normal"))
		->texture("oldReflectionPositionTexture", raytracePass->get("reflectivePosition"))
        ->update("resolution", getResolution(window))
        ->update("gradientDescentSteps", gradientDescentSteps+=1);
		;

    auto tonemapping = (new RenderPass(
    	new Quad(),
		new ShaderProgram({"/Filters/fullscreen.vert","/Filters/toneMapperLinear.frag"})))
        ->texture("tex", raytracePass->get("diffuseColor"))
        ->update("resolution", getResolution(window));

    auto tonemappingFlow = (new RenderPass(
    	new Quad(),
		new ShaderProgram({"/Filters/fullscreen.vert","/Filters/toneMapperLinear.frag"}),
		getWidth(window), getHeight(window)))
        ->texture("tex", diffWarp->get("flow"))
        ->update("resolution", getResolution(window));

    // Compositing
    auto compositing = (new RenderPass(
        quadVAO,
        new ShaderProgram({"/Raytracing/raytrace.vert", "/Raytracing/compositing.frag"}),
        getWidth(window), getHeight(window)))
        ->texture("gatherRefColTexture", gatherRefPass->get("warpedColor"))
        ->texture("warpDiffColTexture",  diffWarp->get("diffuse"))
        ;

    // Compositing
    auto groundTruthCompositing = (new RenderPass(
    	quadVAO,
    	new ShaderProgram({"/Raytracing/raytrace.vert", "/Raytracing/compositing.frag"}),
		getWidth(window), getHeight(window)))
		->texture("gatherRefColTexture", raytracePass->get("reflectiveColor"))
		->texture("warpDiffColTexture",  raytracePass->get("diffuseColor"))
		;

    float rotX = -0.3f;
    float rotY = -0.6f;
    float rotSpeed = 0.15f;

    mat4 view = translate(mat4(1), vec3(0,0,-4)) * eulerAngleXY(-rotX, -rotY);
    mat4 refMatrix = view;

    int mode = 0;
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
                mode = 0;
                 tonemapping->texture("tex", gatherRefPass->get("warpedColor"));
                 break;
            case GLFW_KEY_G:
            	mode = 1;
                 break;
            case GLFW_KEY_H:
            	mode = 2;
                 break;
            case GLFW_KEY_Q:
                // gatherRefPass->texture("warpedNormalTexture", smoothNorm->get("fragColor"));
                tonemapping->texture("tex", diffWarp->get("reflection"));
                 break;
            case GLFW_KEY_W:
                // gatherRefPass->texture("warpedNormalTexture",  diffWarp->get("normal"));
                tonemapping->texture("tex", refWarp->get("position"));
                 break;
            case GLFW_KEY_E:
                 tonemapping->texture("tex", refWarp->get("refColor"));
                 break;
            case GLFW_KEY_R:
                 tonemapping->texture("tex", refWarp->get("uv"));
                 break;
            case GLFW_KEY_N:
                 tonemapping->texture("tex", raytracePassNormal->get("normal"));
                 break;
            case GLFW_KEY_O:
                gatherRefPass->texture("temp", refWarp->get("uv"));
                 break;
            case GLFW_KEY_P:
                gatherRefPass->texture("temp", diffWarp->get("uv"));
                 break;
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_A:
                zoom+=0.3;
                 break;
            case GLFW_KEY_D:
                zoom-=0.3;
                 break;
            case GLFW_KEY_X:
                tonemapping->texture("tex", groundTruthCompositing->get("fragColor"));
                break;
            case GLFW_KEY_Z:
                tonemapping->texture("tex", compositing->get("fragColor"));
                tonemapping->texture("tex", smoothNormal->get("fragColor"));
                break;
            case GLFW_KEY_C:
                gatherRefPass->update("gradientDescentSteps", gradientDescentSteps-=10);
                std::cout << "gradientDescentSteps " << gradientDescentSteps << std::endl;
                break;
            case GLFW_KEY_V:
                gatherRefPass->update("gradientDescentSteps", gradientDescentSteps+=10);
                std::cout << "gradientDescentSteps " << gradientDescentSteps << std::endl;
                break;
            case GLFW_KEY_B:
            	refMatrix = view;

                mat4 view_old = refMatrix; //latencyQueue.front();
//                mat4 invView = inverse(view);
                mat4 invView_old = inverse(view_old);

                mat4 projection = perspective(45.0f + zoom, float(getWidth(window))/float(getHeight(window)), 0.001f, 100.0f);
//                mat4 invViewProjection = inverse(projection * view);
                mat4 invViewProjection_old = inverse(projection * view_old);
//                mat4 vp_old = projection * view_old;

                //bind mesh
              //  ssbo2->bind(11);

                raytracePassNormal
                ->clear(0, 0, 0, 0)
                ->update("view", view_old)
                ->update("projection", projection)
                ->update("invView",invView_old)
                ->run();

                smoothNormal
                ->clear(0, 0, 0, 0)
                ->run();

                raytracePass
                ->clear(0, 0, 0, 0)
                ->update("view", view_old)
                ->update("projection", projection)
                ->update("invViewProjection", invViewProjection_old)
                ->update("invView",invView_old)
                ->run();

                break;
            }
        }
    });


    render(window, [&] (float deltaTime) {
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) (rotY - deltaTime * rotSpeed < 0)? rotY -= deltaTime * rotSpeed + 6.283 : rotY -= deltaTime * rotSpeed;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) (rotY + deltaTime * rotSpeed > 6.283)? rotY += deltaTime * rotSpeed - 6.283 : rotY += deltaTime * rotSpeed;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) (rotX - deltaTime * rotSpeed < 0)? rotX -= deltaTime * rotSpeed + 6.283 : rotX -= deltaTime * rotSpeed;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) (rotX + deltaTime * rotSpeed > 6.283)? rotX += deltaTime * rotSpeed - 6.283 : rotX += deltaTime * rotSpeed;

        view = translate(mat4(1), vec3(0.1,-0.25,-4)) * eulerAngleXY(-rotX, -rotY);

//        // Latency simulation
//        latencyQueue.push(view);
//        while (latencyQueue.size() > latencyFrameNumber) {
//            latencyQueue.pop();
//        }


//        mat4 view_old = refMatrix; //latencyQueue.front();
//        mat4 invView = inverse(view);
//        mat4 invView_old = inverse(view_old);
//
        mat4 projection = perspective(45.0f + zoom, float(getWidth(window))/float(getHeight(window)), 0.001f, 100.0f);
//        mat4 invViewProjection = inverse(projection * view);
//        mat4 invViewProjection_old = inverse(projection * view_old);
//        mat4 vp_old = projection * view_old;

        //bind mesh
      //  ssbo2->bind(11);

//        raytracePass
//        ->clear(0, 0, 0, 0)
//        ->update("view", view_old)
//        ->update("projection", projection)
//        ->update("invViewProjection", invViewProjection_old)
//        ->update("invView",invView_old)
//        ->run();
//
//        raytracePassNormal
//        ->clear(0, 0, 0, 0)
//        ->update("view", view_old)
//        ->update("projection", projection)
//        ->update("invView",invView_old)
//        ->run();

        diffWarp
        ->clear()
        ->update("view", view)
        ->update("projection", projection)
        ->run();

        holeFill
		->clear(1,0,0,0)
		->run();

        refWarp
		->clear()
        ->update("altView", view)
        ->update("projection", projection)
		->run();

        gatherRefPass
        ->clear()
        ->update("view", view)
        ->run();

        compositing
		->clear()
		->run();

        groundTruthCompositing
        ->clear()
        ->run();

        tonemapping
        ->clear()
        ->run();
    });
}
