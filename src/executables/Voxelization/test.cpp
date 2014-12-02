#include "ShaderTools/DefaultRenderLoop.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"

#define PI 3.14159265359f

// create 32bit uint bitmask
GLuint createBitMask()
{
	GLuint bitmask = 0;

	// 32 bit values, one bit set per entry
	unsigned long int bitmaskData[32] =
			{ 1u, 2u, 4u, 8u, 16u, 32u, 64u, 128u,
			256u, 512u, 1024u, 2048u, 4096u, 8192u, 16384u, 32768u, 65536u,
			131072u, 262144u, 524288u, 1048576u, 2097152u, 4194304u, 8388608u,
			16777216u, 33554432u, 67108864u, 134217728u, 268435456u, 536870912u,
			1073741824u, 2147483648u };

	glGenTextures(1, &bitmask);
	glBindTexture(GL_TEXTURE_1D, bitmask);

	// allocate memory:  1D Texture,  1 level,   long uint format (32bit)
	glTexStorage1D( GL_TEXTURE_1D, 1, GL_R32UI, 32);
	glTexSubImage1D( GL_TEXTURE_1D, 0, 0, 32, GL_RED_INTEGER, GL_UNSIGNED_INT, &bitmaskData);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_1D, 0);

	return bitmask;
}

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

GLuint bitmask = createBitMask();

float near = 0.1f;
float far  = 6.0f;

float red = 0.0f;
float green = 1.0f;
float blue = 0.0f;

glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = glm::lookAt(glm::vec3(0.0f,0.0f,3.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f) );
glm::mat4 projection = glm::perspective(60.0f * PI / 180.0f, (float) width/height, near, far );

int main(int argc, char *argv[]) {
    sp -> printUniformInfo();
    sp -> printInputInfo();
    sp -> printOutputInfo();

    //TODO framebuffers

    renderLoop([]{

  		//TODO : Framelimiter

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {view = glm::translate(glm::vec3(0.0f,0.0f,0.01f)) * view;}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {view = glm::translate(glm::vec3(0.0f,0.0f,-0.01f)) * view;}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {view = glm::translate(glm::vec3(-0.01f, 0.0f,0.0f)) * view;}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {view = glm::translate(glm::vec3(0.01f,0.0f,0.0f)) * view;}
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {glfwDestroyWindow(window); exit(-1);}

		// render slicemap
		slicemappingPass
		->clear(0,0,0,0)
		->update("near", near)
		->update("far", far)
		->update("numSliceMaps", 4)
		->texture("bitMask", bitmask) // does this even work?
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
