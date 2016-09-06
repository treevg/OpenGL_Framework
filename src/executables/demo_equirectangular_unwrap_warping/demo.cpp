#include "ShaderTools/Renderer.h"
#include "ShaderTools/RenderPass.h"
#include "AssetTools/Texture.h"
#include "AssetTools/Scene.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "ShaderTools/VertexArrayObjects/Grid.h"
#include "RenderTechniques/Pyramid.h"

using namespace std;
using namespace glm;

int main(int argc, char *argv[]) {
    GLFWwindow* window = generateWindow(1024,1024);

    mat4 projection = perspective(0.4f * 3.142f, getRatio(window), 0.1f, 1000.0f);

    auto skyTex = Texture(RESOURCES_PATH "/textures/cubeTexture.jpg");
    auto scene = new Scene(RESOURCES_PATH "/scenes/sibenik/sibenik.obj");
    auto quad = new Quad();

    RenderPass* objects = (new RenderPass(
        new ShaderProgram("/3DObject/modelViewProjection.vert","/3DObject/gBuffer.frag")
        ,getWidth(window), getHeight(window)))
            ->update("projection", projection);

    ShaderProgram* unwrapShader = new ShaderProgram("/Filters/fullscreen.vert","/Reprojection/equirectangularUnwrap.frag");

    float resolutionFactor = 4;
    RenderPass* unwrapColor = (new RenderPass(
        quad, unwrapShader, getWidth(window) * resolutionFactor, getHeight(window) * resolutionFactor))
            ->depthTest(false)
            ->update("resolution", getResolution(window) * resolutionFactor)
            ->update("projection", projection);

    RenderPass* unwrapPosition = (new RenderPass(
        quad, unwrapShader, getWidth(window) * resolutionFactor, getHeight(window) * resolutionFactor))
            ->depthTest(false)
            ->update("resolution", getResolution(window) * resolutionFactor)
            ->update("projection", projection);

    ShaderProgram* remapShader = new ShaderProgram("/Filters/fullscreen.vert","/3DObject/equirectangularSky.frag");

    RenderPass* remapColor = (new RenderPass(
        quad, remapShader, getWidth(window), getHeight(window)))
            ->update("resolution", getResolution(window))
            ->update("projection", projection);

    RenderPass* remapPosition = (new RenderPass(
        quad, remapShader, getWidth(window), getHeight(window)))
            ->update("resolution", getResolution(window))
            ->update("projection", projection);

    RenderPass* reproject = (new RenderPass(
        new Grid(getWidth(window), getHeight(window)),
        new ShaderProgram("/Reprojection/reprojectMVP.vert","/3DObject/gBuffer.frag"),
        getWidth(window), getHeight(window)))
            ->update("projection", projection);

    RenderPass* output = (new RenderPass(
        quad,
        new ShaderProgram("/Filters/fullscreen.vert","/Filters/toneMapperLinearResize.frag")))
        ->update("resolution", getResolution(window))
        ->depthTest(false);

    Pyramid* holefilling = new Pyramid(getWidth(window), getHeight(window),
        "/RenderTechniques/Pyramid/pullHolefilling.frag",
        "/RenderTechniques/Pyramid/pushHolefilling.frag");

    float rotationSensitivity = 0.01;
    vec3 viewRotationAngles = vec3(-0.4, 0.4, 0);
    vec2 cursorPosition = vec2(0);
    mat4 view;

    auto calcView = [&] () {
        view = rotate(viewRotationAngles.z, vec3(0,0,1)) * eulerAngleXY(viewRotationAngles.y, viewRotationAngles.x) * translate(mat4(1), vec3(4, 10, 0));
    };

    setCursorPosCallback(window, [&] (double xpos, double ypos) {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            viewRotationAngles += vec3(vec2(xpos, ypos) - cursorPosition, 0) * rotationSensitivity * 0.5;
        }
        cursorPosition = vec2(xpos, ypos);
        calcView();
    });

    setScrollCallback(window, [&] (double xoffset, double yoffset){
        viewRotationAngles.z += yoffset * rotationSensitivity * 2;
        calcView();
    });


    int show = 0;
    bool toggleHolefilling = false;
    float toggleLeftRightEye = 1.0;
    float eyeDistance = 0.5;
    setKeyCallback(window, [&] (int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            if (key >= GLFW_KEY_1 && key <= GLFW_KEY_8) show = key - GLFW_KEY_1;
            if (key == GLFW_KEY_H) toggleHolefilling = !toggleHolefilling;
            if (key == GLFW_KEY_E) toggleLeftRightEye = -toggleLeftRightEye;
            if (key == GLFW_KEY_LEFT) eyeDistance -= 0.05;
            if (key == GLFW_KEY_RIGHT) eyeDistance += 0.05;
        }
    });

    glPointSize(2);

    render(window, [&] (float deltaTime) {
        objects
        ->clear()
        ->update("view", view)
        ->clearDepth();

        for (auto o : scene->objects) {
            objects
            ->texture("tex", o->texture)
            ->update("model", glm::mat4(1))
            ->run(o->mesh);
        }

        unwrapColor
        ->texture("tex", objects->get("color"))
        ->update("view", view)
        ->run();

        unwrapPosition
        ->texture("tex", objects->get("position"))
        ->update("view", view)
        ->run();

        remapColor
        ->clear()
        ->texture("tex", unwrapColor->get("fragColor"))
        ->update("view", view)
        ->run();

        remapPosition
        ->clear()
        ->texture("tex", unwrapPosition->get("fragColor"))
        ->update("view", view)
        ->run();

        output
        ->clear()
        ->clearDepth();

        switch (show) {
            case 0: {
                output
                // ->update("resolution", getResolution(window))
                ->texture("tex", objects->get("color"))
                ->update("minRange", 0.0)
                ->update("maxRange", 1.0)
                ->run();

                break;
            }

            case 1: {
                output
                // ->update("resolution", getResolution(window))
                ->texture("tex", objects->get("position"))
                ->update("minRange", -10.0)
                ->update("maxRange", 10.0)
                ->run();

                break;
            }

            case 2: {
                output
                // ->update("resolution", getResolution(window))
                ->texture("tex", unwrapColor->get("fragColor"))
                ->update("minRange", 0.0)
                ->update("maxRange", 1.0)
                ->run();

                break;
            }

            case 3: {
                output
                // ->update("resolution", getResolution(window))
                ->texture("tex", unwrapPosition->get("fragColor"))
                ->update("minRange", -10.0)
                ->update("maxRange", 10.0)
                ->run();

                break;
            }

            case 4: {
                output
                // ->update("resolution", getResolution(window))
                ->texture("tex", remapColor->get("fragColor"))
                ->update("minRange", 0.0)
                ->update("maxRange", 1.0)
                ->run();
                break;
            }

            case 5: {
                output
                // ->update("resolution", getResolution(window))
                ->texture("tex", remapPosition->get("fragColor"))
                ->update("minRange", -10.0)
                ->update("maxRange", 10.0)
                ->run();
                break;
            }

            case 6: {
                reproject
                ->clear()
                ->texture("worldPosition", remapPosition->get("fragColor"))
                ->texture("tex", remapPosition->get("fragColor"))
                ->update("view", translate(mat4(1), vec3(toggleLeftRightEye * eyeDistance * 0.5,0,0)) * view)
                ->run();

                if (toggleHolefilling) {
                    holefilling
                    ->texture("tex", reproject->get("color"))
                    ->run();

                    output
                    ->texture("tex", holefilling->get("fragColor"));
                } else {
                    output
                    ->texture("tex", reproject->get("color"));
                }

                output
                // ->update("resolution", getResolution(window))
                ->update("minRange", -10.0)
                ->update("maxRange", 10.0)
                ->run();
                break;
            }

            case 7: {
                reproject
                ->clear()
                ->texture("worldPosition", remapPosition->get("fragColor"))
                ->texture("tex", remapColor->get("fragColor"))
                ->update("view", translate(mat4(1), vec3(toggleLeftRightEye * eyeDistance * 0.5,0,0)) * view)
                ->run();

                if (toggleHolefilling) {
                    holefilling
                    ->texture("tex", reproject->get("color"))
                    ->run();

                    output
                    ->texture("tex", holefilling->get("fragColor"));
                } else {
                    output
                    ->texture("tex", reproject->get("color"));
                }

                output
                // ->update("resolution", getResolution(window))
                ->update("minRange", 0.0)
                ->update("maxRange", 1.0)
                ->run();
                break;
            }
        }
    });
}
