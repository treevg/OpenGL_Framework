#include "ShaderTools/DefaultRenderLoop.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"

#include "VoxelizationTools/SlicemapUtilities.h"
#include "VoxelizationTools/OpenGLTestUtilities.h"

#define PI 3.14159265359f

// DESCRIPTION
/*
 * This program creates a 128-slices slicemap from an object encoded in a RGBA texture of a 32bit unsigned integer format
 * This slicemap is interpreted as a transmissive shadowmap, which simulates partial light absorption by transmissive material like paper or foliage.
 */

// SHADER PROGRAMS
auto sp = new ShaderProgram({"/Voxelization/simpleVertex.vert", "/Voxelization/simpleColoring.frag"});
auto slicemappingShader = new ShaderProgram({"/Voxelization/simpleVertex.vert", "/Voxelization/sliceMap.frag"});
auto projectSlicemap	= new ShaderProgram({"/Voxelization/screenfill.vert", "/Voxelization/sliceMapOverlay.frag"});

// OBJECTS
auto cube = new Cube();
auto quad = new Quad();

// RENDERPASSES
auto pass = new RenderPass( cube, sp, width, height );	// render cube
auto slicemappingPass = new SlicemapRenderPass( cube, slicemappingShader, width, height ); // render slice map
auto overlaySlicemapPass = new RenderPass( quad, projectSlicemap);	// project slice map onto rendered scene

// GLOBAL VARIABLES
float near = 0.1f;  // clipping planes
float far  = 6.0f;  // of voxelization camera

float red = 0.25f;   // object colors
float green = 0.25f;
float blue = 0.25f;

float backgroundTransparency = 0.5f;	// visibility of cube

glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = glm::lookAt(glm::vec3(0.0f,0.0f,3.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f) );
glm::mat4 projection = glm::perspective(60.0f * PI / 180.0f, (float) width/height, near, far );

GLuint bitmask = createRGBA32UIBitMask();	// handle of 1D bitmask texture
GLuint clearSlicemap[4] = {0, 0, 0, 0};

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
		if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {backgroundTransparency = glm::clamp( backgroundTransparency + 0.02f, 0.0f, 1.0f);}
		if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {backgroundTransparency = glm::clamp( backgroundTransparency - 0.02f, 0.0f, 1.0f);}

		// clear slicemap
		glBindFramebuffer( GL_FRAMEBUFFER, slicemappingPass->frameBufferObject->getFrameBufferObjectHandle());
		glClearBufferuiv(GL_COLOR, 0, clearSlicemap);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// enable bitwise logic OR operation on outgoing fragments and disable depth test
		glEnable(GL_COLOR_LOGIC_OP);
		glLogicOp(GL_OR);

		// bind bitmask to image unit 0
//		glBindImageTexture( 0, bitmask, 0, GL_FALSE,	0, GL_READ_ONLY, GL_RGBA32UI );

		// bind bitmask to texture unit 0
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_1D, bitmask);

//		startTime();
		// render slicemap
		slicemappingPass
		->update("model", model)
		->update("view", view)
		->update("projection", projection)
		->update("near", near)
		->update("far", far)
		->run();
//		stopTime();

		// restore default values
		glBindTexture(GL_TEXTURE_1D, 0);
		glDisable(GL_COLOR_LOGIC_OP);
		glLogicOp(GL_COPY);

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

        //bind slicemap to image unit 0
        glBindImageTexture(0, slicemappingPass->get("slice0_127"), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32UI );

//		startTime();
        // overlay slicemap onto scene for visualization
		overlaySlicemapPass
        ->clear(0,0,0,0)
        ->texture("baseTexture", pass->get("fragmentColor"))
        ->update("backgroundTransparency", backgroundTransparency)
        ->run();
//		stopTime();

		// unbind image units (clean up)
		glBindImageTexture(0,0,0,GL_FALSE,0,GL_READ_WRITE,GL_RGBA32UI);
    });
}
