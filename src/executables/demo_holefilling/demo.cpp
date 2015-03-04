#include "ShaderTools/Renderer.h"
#include "ShaderTools/RenderPass.h"
#include "AssetTools/Texture.h"
#include "AssetTools/Mesh.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "ShaderTools/VertexArrayObjects/Grid.h"

using namespace std;
using namespace glm;

int main(int argc, char *argv[]) {
    GLFWwindow* window = generateWindow();

    float rotX = 0.0f;
    float rotY = 0.0f;
    int numMipmaps = 10;

    mat4 projection = perspective(45.0f, getRatio(window), 0.1f, 100.0f);

    auto texture = new Texture(RESOURCES_PATH "/equirectangular/plaza.png");
    auto quad = new Quad();

    auto sparse = (new RenderPass(
        new Grid(100, 100), 
        new ShaderProgram({"/3DObject/modelViewProjection.vert","/3DObject/equirectangularSky.frag"}),
        getWidth(window), getHeight(window)))
            ->texture("tex", texture->getHandle())
            ->update("resolution", getResolution(window))
            ->update("model", mat4(1))
            ->update("projection", projection);

    GLuint mipmap;

    glGenTextures(1, &mipmap);
    glBindTexture(GL_TEXTURE_2D, mipmap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, getWidth(window), getHeight(window), 0, GL_RGBA, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glGenerateMipmap(GL_TEXTURE_2D);

    auto mipmapFBOHandles = new GLuint[numMipmaps];
    glGenFramebuffers(numMipmaps, mipmapFBOHandles);

    vector<FrameBufferObject*> mipmapFBOs;
    for (int i = 0; i < numMipmaps; i++) {
        auto fbo = new FrameBufferObject();
        fbo->setFrameBufferObjectHandle(mipmapFBOHandles[i]);
        mipmapFBOs.push_back(fbo);
    }

    for (int i = 0; i < numMipmaps; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, (mipmapFBOHandles)[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mipmap, i);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
    }

    auto reduce = (new RenderPass(
        quad, 
        new ShaderProgram({"/Filters/fullscreen.vert","/Filters/hhfReduce.frag"})))
            ->texture("mipmapTexture", sparse->get("fragColor"));

    auto fill = (new RenderPass(
        quad, 
        new ShaderProgram({"/Filters/fullscreen.vert","/Filters/hhfFill.frag"})))
            ->texture("mipmapTexture", mipmap)
            ->update("resolution", getResolution(window));

    auto tonemapping = (new RenderPass(
        quad, 
        new ShaderProgram({"/Filters/fullscreen.vert","/Filters/toneMapperLinear.frag"})))
            ->texture("tex", mipmap)
            // ->texture("tex", mipmap)
            ->update("minRange", 0.0)
            ->update("maxRange", 1.0);

    int level = 0;
    bool toggleInOut = true;
    setKeyCallback(window, [&] (int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            switch (key) {
            case GLFW_KEY_PERIOD:
                level = clamp(level + 1, 0, numMipmaps-1);
                cout << level << endl;
                tonemapping->update("level", level);
                break;
            case GLFW_KEY_COMMA:
                level = clamp(level - 1, 0, numMipmaps-1);
                cout << level << endl;
                tonemapping->update("level", level);
                break;
            case GLFW_KEY_SPACE:
                if (toggleInOut) {
                    tonemapping->texture("tex", mipmap);
                    cout << mipmap << endl;
                } else {
                    tonemapping->texture("tex", sparse->get("fragColor"));
                    cout << sparse->get("fragColor") << endl;
                }
                toggleInOut = !toggleInOut;
                break;
            } 
        }
    });


    render(window, [&] (float deltaTime) {
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) (rotY - deltaTime < 0)? rotY -= deltaTime + 6.283 : rotY -= deltaTime;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) (rotY + deltaTime > 6.283)? rotY += deltaTime - 6.283 : rotY += deltaTime;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) (rotX - deltaTime < 0)? rotX -= deltaTime + 6.283 : rotX -= deltaTime;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) (rotX + deltaTime > 6.283)? rotX += deltaTime - 6.283 : rotX += deltaTime;

        sparse
            ->update("view", translate(mat4(1), vec3(-0.5,-0.5,-1)) * eulerAngleXY(-rotX, -rotY))
            ->clear()
            ->run();

        glGenerateMipmap(GL_TEXTURE_2D);

        reduce->texture("mipmapTexture", sparse->get("fragColor"));
        for (int i = 0; i < numMipmaps; i++) {
            reduce->update("level", i);
            reduce->frameBufferObject = mipmapFBOs[i];
            reduce
                ->run()
                ->texture("mipmapTexture", mipmap);
        }

        for (int i = numMipmaps-1; i >= 0; i--) {
            fill->update("level", i);
            fill->frameBufferObject = mipmapFBOs[i];
            fill->run();
        }

        tonemapping
            ->clear()
            ->run();
    });
}