#include "ShaderTools/DefaultRenderLoop.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"

#define PI 3.14159265359f

auto sp = new ShaderProgram({"/Voxelization/simpleVertex.vert", "/Voxelization/simpleColoring.frag"});
auto slicemappingShader = new ShaderProgram({"/Voxelization/simpleVertex.vert", "/Voxelization/sliceMapMultipleTargets.frag"});

auto cube = new Cube();

auto pass = new RenderPass(
    cube,
    sp
);

auto slicemappingPass = new RenderPass(
    cube,
    slicemappingShader
);

GLuint bitmask;

float near = 0.1f;
float far  = 6.0f;

float red = 0.0f;
float green = 1.0f;
float blue = 0.0f;


glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = glm::lookAt(glm::vec3(0.0f,0.0f,3.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f) );
glm::mat4 projection = glm::perspective(60.0f * PI / 180.0f, (float) width/height, near, far );

GLuint createBitMask()
{
	//TODO
	return 0;
}

int main(int argc, char *argv[]) {
    sp -> printUniformInfo();
    sp -> printInputInfo();
    sp -> printOutputInfo();

    //create bitmask
    bitmask = createBitMask();

    renderLoop([]{

  		//TODO : Framelimiter

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {view = glm::translate(0.0f,0.0f,0.01f) * view;}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {view = glm::translate(0.0f,0.0f,-0.01f) * view;}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {view = glm::translate(-0.01f, 0.0f,0.0f) * view;}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {view = glm::translate(0.01f,0.0f,0.0f) * view;}
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {glfwDestroyWindow(window); exit(-1);}

		// render slicemap
		slicemappingPass
		->clear(0,0,0,0)
		->update("near", near)
		->update("far", far)
		->update("numSliceMaps", 4)
		->texture("bitMask", bitmask)
		->run();

		// render scene
        pass
        -> clear(0, 0, 0, 0)
        -> update("model", model)
        -> update("view", view)
        -> update("projection", projection)
        -> update("red", red)
        -> update("green", green)
        -> update("blue", blue)
        -> update("alpha", 1.0f)
        -> run();

        // project slicemap

    });
}
