#include "ShaderTools/Renderer.h"
#include "ShaderTools/RenderPass.h"
#include "AssetTools/Texture.h"
#include "ShaderTools/VertexArrayObjects/Grid.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "RenderTechniques/Pyramid.h"

using namespace std;
using namespace glm;

int main(int argc, char *argv[]) {
    GLFWwindow* window = generateWindow(512, 512);

    Texture* image = new Texture("/home/lochmann/tmp/blupp/out.jpg");

    auto pyramid = (new Pyramid(getWidth(window), getHeight(window), 
        "/RenderTechniques/Pyramid/pullGaussian.frag",
        "/RenderTechniques/Pyramid/pushBSpline.frag"))
            ->texture("inputTex", image->getHandle());


    auto dumbBoxBlur = (new RenderPass(
        new Quad(), 
        new ShaderProgram("/Filters/fullscreen.vert","/Filters/boxBlur.frag"), 
        getWidth(window), 
        getHeight(window)))
            ->texture("tex", image->getHandle());

    auto tonemapping = (new RenderPass(
        new Quad(),
        new ShaderProgram("/Filters/fullscreen.vert","/Filters/toneMapperLinear.frag")))
            ->texture("tex", dumbBoxBlur->get("fragColor"));

    int level = 0;
    int limit = 0;
    bool toggleInOut = true;
    setKeyCallback(window, [&] (int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            switch (key) {
            case GLFW_KEY_COMMA:
                dumbBoxBlur->update("strength", (level < 20)? ++level : level);
                break;
            case GLFW_KEY_PERIOD:
                dumbBoxBlur->update("strength", (level > 0)? --level : level);
                break;
            case GLFW_KEY_M:
                pyramid->setLimit((level < pyramid->getMipmapNumber()-1)? ++level : level);
                break;
            case GLFW_KEY_N:
                pyramid->setLimit((level > 0)? --level : level);
                break;
            case GLFW_KEY_SPACE:
                (toggleInOut ^= 1);//? tonemapping->texture("tex", pyramid->get("fragColor")) : tonemapping->texture("tex", pyramid->get("fragPosition"));
                break;
            } 
        }
    });



/*
{ //MEASURING STUFF
    float timer;
    float diff;
    int retries = 3;
    
    std::string path = "/home/lochmann/tmp/blupp/out.jpg";

    ILuint iid;
    ilGenImages(1, &iid);
    ilBindImage(iid);
    ilLoadImage(path.c_str());

    int w = ilGetInteger(IL_IMAGE_WIDTH);
    int h = ilGetInteger(IL_IMAGE_HEIGHT);
    const int byteCount = sizeof(float) * 3 * w * h;
    unsigned char* pixels = new unsigned char[byteCount];
    ilCopyPixels(0, 0, 0, w, h, 1, IL_RGB, IL_FLOAT, pixels);

    glFinish();
    GLuint t;
    glGenTextures(1, &t);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
    for (int i = 0; i < retries; i++) {
        glFinish();
        timer = glfwGetTime();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGB, GL_FLOAT, pixels);
        glFinish();
        diff = glfwGetTime() - timer;
        printf("timer: load texture to gpu: %f\n", diff);
    }
    glBindImageTexture(0, t, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA16F);

    GLubyte *pixels2 = (GLubyte*)malloc(512*512*4*sizeof(GLubyte));
    glBindTexture(GL_TEXTURE_2D,t);
    for (int i = 0; i < retries; i++) {
        glFinish();    
        timer = glfwGetTime();
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels2);
        glFinish();
        diff = glfwGetTime() - timer;
        printf("timer: load texture to memory: %f\n", diff);
    }

    // for (int i = 0; i < 100; i+=4) {
    //     printf("pixel data: %d %d %d %d\n", pixels2[i], pixels2[i+1], pixels2[i+2], pixels2[i+3]);
    // }

    timer = glfwGetTime();
    int r = 0;
    int g = 0;
    int b = 0;
    int a = 0;
    for (int i = 0; i < 512*512*4; i+=4) {
        r += pixels2[i];
        g += pixels2[i+1];
        b += pixels2[i+2];
        a += pixels2[i+3];
    }
    r /= 512*512;
    g /= 512*512;
    b /= 512*512;
    a /= 512*512;

    diff = glfwGetTime() - timer;
    printf("timer: cpu compute average: %f\n", diff);
    printf("result: %i %i %i %i\n", r, g, b, a);


    glFinish();
    timer = glfwGetTime();
    pyramid
        ->pull();
    glFinish();
    diff = glfwGetTime() - timer;

    printf("timer: gpu compute average: %f\n", diff);

    GLubyte *pixels3 = (GLubyte*)malloc(4*sizeof(GLubyte));
    glBindTexture(GL_TEXTURE_2D,pyramid->get("fragColor"));
    for (int i = 0; i < retries; i++) {
        glFinish();
        timer = glfwGetTime();
        glGetTexImage(GL_TEXTURE_2D, pyramid->getMipmapNumber() - 1, GL_RGBA, GL_UNSIGNED_BYTE, pixels3);
        glFinish();
        r = 0;
        g = 0;
        b = 0;
        a = 0;
        r += pixels3[0];
        g += pixels3[1];
        b += pixels3[2];
        a += pixels3[3];
        diff = glfwGetTime() - timer;
        printf("timer: load texture to memory: %f\n", diff);
    }
    printf("result: %i %i %i %i\n", r, g, b, a);
}
*/

    render(window, [&] (float deltaTime) {
        float timer;
        float diff;
        glFinish();
        timer = glfwGetTime();

        dumbBoxBlur
            ->clear()
            ->run();

        // pyramid->pull();

        // // if(toggleInOut)
        //     pyramid->push();

        glFinish();
        diff = glfwGetTime() - timer;
        printf("timer: blurring: %f\n", diff);

        tonemapping
            ->clear()
            ->run();
    });
}