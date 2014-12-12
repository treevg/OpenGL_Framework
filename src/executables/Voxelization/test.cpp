#include "ShaderTools/DefaultRenderLoop.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"

#include "SlicemapUtilities.h"

#define PI 3.14159265359f

/*TODO
 * - enable slicemapping shader to use 128 slices
 * - test whether 1D Texture can still be used as 2D-Texture ( else hardcode )
 * - enable slicemapoverlay shader to use 128 slices
 */

// DESCRIPTION
/*
 * This program creates a 128-slices slicemap from an object encoded in a RGBA texture of a 32bit unsigned integer format
 * For simplicity, the voxelization camera and the first person camera use the same matrices and the slicemap is overlayed additively onto the scene.
 */

// SHADER PROGRAMS
auto sp = new ShaderProgram({"/Voxelization/simpleVertex.vert", "/Voxelization/simpleColoring.frag"});
auto slicemappingShader = new ShaderProgram({"/Voxelization/simpleVertex.vert", "/Voxelization/sliceMapMultipleTargets.frag"});
auto projectSlicemap	= new ShaderProgram({"/Voxelization/screenfill.vert", "/Voxelization/sliceMapOverlay.frag"});

// OBJECTS
auto cube = new Cube();
auto quad = new Quad();

// RENDERPASSES
auto pass = new RenderPass( cube, sp, width, height );	// render cube
auto slicemappingPass = new SlicemapRenderPass( cube, slicemappingShader, width, height ); // render slice map
auto projectSlicemapPass = new RenderPass( quad, projectSlicemap);	// project slice map onto rendered scene
GLuint bitmask = createRGBA32UIBitMask();

// GLOBAL VARIABLES
float near = 0.1f;  // clipping planes
float far  = 6.0f;  // of voxelization camera

float red = 0.25f;   // object colors
float green = 0.25f;
float blue = 0.25f;

bool enabled = true;
float backgroundTransparency = 0.25f;

int numSlicemaps = 1;

glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = glm::lookAt(glm::vec3(0.0f,0.0f,3.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f) );
glm::mat4 projection = glm::perspective(60.0f * PI / 180.0f, (float) width/height, near, far );

int main(int argc, char *argv[]) {
    sp -> printUniformInfo();
    sp -> printInputInfo();
    sp -> printOutputInfo();

    slicemappingShader->printUniformInfo();
    slicemappingShader->printInputInfo();
    slicemappingShader->printOutputInfo();

    projectSlicemap->printUniformInfo();
    projectSlicemap->printInputInfo();
    projectSlicemap->printOutputInfo();

    renderLoop([]{
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {view = glm::translate(glm::vec3(0.0f,0.0f,0.01f)) * view;}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {view = glm::translate(glm::vec3(0.0f,0.0f,-0.01f)) * view;}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {view = glm::translate(glm::vec3(-0.01f, 0.0f,0.0f)) * view;}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {view = glm::translate(glm::vec3(0.01f,0.0f,0.0f)) * view;}
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {glfwDestroyWindow(window); exit(-1);}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {enabled = !enabled;}
		if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {backgroundTransparency += 0.15;}
		if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {backgroundTransparency -= 0.15;}

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
//		->update("near", near)
//		->update("far", far)
        -> run();


//TODO  bind bitmask to an image unit instead of using texture method
		// render slicemap
		slicemappingPass
		->clear(0,0,0,0)
        ->update("model", model)
        ->update("view", view)
        ->update("projection", projection)
		->update("near", near)
		->update("far", far)
		->update("numSlicemaps", numSlicemaps)
		->texture("bitMask", bitmask) // does this even work?
		->run();

		// display slicemap

//TODO  bind slice map to image unit instead of using texture method

		projectSlicemapPass
        ->clear(0,0,0,0)
        ->texture("baseTexture", pass->get("fragmentColor"))
//       ->texture("sliceMapTexture", slicemappingPass->get("slice0_127"))
//       ->update("enabled", enabled)
        ->update("backgroundTransparency", backgroundTransparency)
        ->run();
    });
}
