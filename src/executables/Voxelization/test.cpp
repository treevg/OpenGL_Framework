#include "ShaderTools/DefaultRenderLoop.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"

#include "SlicemapUtilities.h"
#include "OpenGLTestUtilities.h"

#define PI 3.14159265359f

/*TODO
 * - fix AMD vs. NVIDIA bug
 * - enable multiple slicemaps
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

// GLOBAL VARIABLES
float near = 0.1f;  // clipping planes
float far  = 6.0f;  // of voxelization camera

float red = 0.25f;   // object colors
float green = 0.25f;
float blue = 0.25f;

bool enabled = true;
float backgroundTransparency = 0.5f;

int numSlicemaps = 1;

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
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {enabled = !enabled;}
		if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {backgroundTransparency = glm::clamp( backgroundTransparency + 0.1f, 0.0f, 1.0f);}
		if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {backgroundTransparency = glm::clamp( backgroundTransparency - 0.1f, 0.0f, 1.0f);}

			// clear slicemap
			glBindFramebuffer( GL_FRAMEBUFFER, slicemappingPass->frameBufferObject->getFrameBufferObjectHandle());
			glClearBufferuiv(GL_COLOR, 0, clearSlicemap);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			// enable bitwise logic OR operation on outgoing fragments
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_COLOR_LOGIC_OP);
			glLogicOp(GL_OR);

			// bind bitmask to image unit 0
			glBindImageTexture(0,	// image unit binding
					bitmask,		// texture
					0,				// texture level
					GL_FALSE,		// layered
					0,				// layer
					GL_READ_ONLY,	// access
					GL_RGBA32UI		// format
			);

			// render slicemap
			slicemappingPass
			->update("model", model)
			->update("view", view)
			->update("projection", projection)
			->update("near", near)
			->update("far", far)
//			->update("numSlicemaps", numSlicemaps)
			->run();

			// restore default values
			glEnable(GL_DEPTH_TEST);
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

        //
        glBindImageTexture(1,           // image unit binding
		slicemappingPass->get("slice0_127"), // texture
		0,								// texture level
		GL_FALSE,                       // layered
		0,                              // layer
		GL_READ_ONLY,                   // access
		GL_RGBA32UI                     // format
		);

        // overlay slicemap onto scene for visualization
		projectSlicemapPass
        ->clear(0,0,0,0)
        ->texture("baseTexture", pass->get("fragmentColor"))
        ->update("backgroundTransparency", backgroundTransparency)
        ->run();

		// unbind image units (clean up)
		glBindImageTexture(1,0,0,GL_FALSE,0,GL_READ_WRITE,GL_RGBA32UI);
		glBindImageTexture(0,0,0,GL_FALSE,0,GL_READ_WRITE,GL_RGBA32UI);

    });
}
