#include "ShaderTools/DefaultRenderLoop.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"

#include "VoxelizationTools/SlicemapUtilities.h"
#include "VoxelizationTools/OpenGLTestUtilities.h"

#define PI 3.14159265359f

// DESCRIPTION
/* This program uses a slice map similarly to a shadow map to simulate partial light absorption (i.e. foliage, transmissive material)
 * A 128-slices slice map is used as the render target of the light source's view (see Slicemapping program).
 * A G-Buffer (color, position) is used as the render target of the camera view.
 * A screen-filling quad is rendered to evaluate the shadow map, by counting the set bits between the fragment's position and the light source.
 */

// SHADER PROGRAMS
auto gbufferSp      = new ShaderProgram({"/Voxelization/simpleVertex.vert", "/Voxelization/gbuffer.frag"});
auto slicemappingSp = new ShaderProgram({"/Voxelization/simpleVertex.vert", "/Voxelization/sliceMap.frag"});
auto compositingSp	= new ShaderProgram({"/Voxelization/screenfill.vert",   "/Voxelization/shadowmappingSliceMap.frag"});

// VAO OBJECTS
auto cube = new Cube();
auto quad = new Quad();

// RENDERPASSES
int gridResX = 256;
int gridResY = 256;

auto gbufferPass      = new RenderPass(         cube, gbufferSp,      width, height );	    // render object
auto slicemappingPass = new SlicemapRenderPass( cube, slicemappingSp, gridResX, gridResY ); // render slice map
auto compositingPass  = new RenderPass(         quad, compositingSp);	                    // apply transmissive shadowmap

// VARIABLES / PARAMETERS
float red   = 0.133f;   // object colors
float green = 0.545f;
float blue  = 0.133f;
float opacityPerSlice = 0.15f;

// container for model matrices
std::vector<glm::mat4> model;

// camera parameters
float near = 0.1f;
float far  = 4.0f;
glm::mat4 view = glm::lookAt(glm::vec3(0.0f,0.0f,1.5f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f) );
glm::mat4 projection = glm::perspective(60.0f * PI / 180.0f, (float) width/height, near, far );

// light source (voxelization camera) parameters
float lightNear = 0.0f;
float lightFar  = 1.50f;
glm::mat4 lightView = glm::lookAt(glm::vec3(0.0f,1.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,-1.0f) );
glm::mat4 lightProjection = glm::ortho( -1.5f, 1.5f, -1.5f, 1.5f, lightNear, lightFar );

// utility textures
GLuint bitmask     = createRGBA32UIBitMask();	// handle of 1D bitmask texture
GLuint acc_bitmask = createRGBA32UIAccBitMask();// handle of 1D accumulated bitmask texture
GLuint clearSlicemap[4] = {0, 0, 0, 0};

int main(int argc, char *argv[]) {

	// generate model matrices for small plates and a ground plane
	srand(time(NULL));
	for (int i = 0; i < 75; i++)
	{
		float offX = ( (float( rand()) / float(RAND_MAX) ) - 0.5f ); // -0.5..0.5
		float offY = ( (float( rand()) / float(RAND_MAX) ) * 0.75f - 0.25f) ; // 0.0..0.75
		float offZ = ( (float( rand()) / float(RAND_MAX) ) - 0.5f ); // -0.5..0.5
		model.push_back( glm::translate(glm::mat4(1.0f),glm::vec3(offX,offY,offZ)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f,0.005f,0.1f)) ); // small plane
	}
	model.push_back( glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,-0.5f,0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.25f, 0.01f, 1.25f) ) ); // ground plane

	// print shader info
    gbufferSp -> printUniformInfo();
    gbufferSp -> printInputInfo();
    gbufferSp -> printOutputInfo();
    slicemappingSp->printUniformInfo();
    slicemappingSp->printInputInfo();
    slicemappingSp->printOutputInfo();
    compositingSp->printUniformInfo();
    compositingSp->printInputInfo();
    compositingSp->printOutputInfo();

    renderLoop([]{
		if (glfwGetKey(window, GLFW_KEY_UP) 	== GLFW_PRESS) {view = glm::translate(glm::vec3(0.0f,0.0f,0.01f)) * view;}
		if (glfwGetKey(window, GLFW_KEY_DOWN) 	== GLFW_PRESS) {view = glm::translate(glm::vec3(0.0f,0.0f,-0.01f)) * view;}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) 	== GLFW_PRESS) {view = glm::translate(glm::vec3(-0.01f, 0.0f,0.0f)) * view;}
		if (glfwGetKey(window, GLFW_KEY_LEFT) 	== GLFW_PRESS) {view = glm::translate(glm::vec3(0.01f,0.0f,0.0f)) * view;}
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {glfwDestroyWindow(window); exit(-1);}
		if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {opacityPerSlice = glm::clamp( opacityPerSlice + 0.002f, 0.0f, 1.0f);}
		if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {opacityPerSlice = glm::clamp( opacityPerSlice - 0.002f, 0.0f, 1.0f);}

		// SLICE MAP:
		// clear slice map
		//glBindFramebuffer( GL_FRAMEBUFFER, slicemappingPass->frameBufferObject->getFrameBufferObjectHandle());
		glClearBufferuiv(GL_COLOR, 0, clearSlicemap);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// enable bitwise logic OR operation on outgoing fragments
		glEnable(GL_COLOR_LOGIC_OP);
		glLogicOp(GL_OR);

		// bind bit mask
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_1D, bitmask);

		//adjust viewport to slice map resolution
		glViewport(0,0,gridResX,gridResY);

		// render slice map
		slicemappingPass
		->update("view",       lightView)
		->update("projection", lightProjection)
		->update("near",       lightNear)
		->update("far",        lightFar);
		for ( unsigned int i = 0; i < model.size(); i++ )
		{
			slicemappingPass
					->update( "model", model[i] )
					->run();
		}

		// restore default values
		glBindTexture(GL_TEXTURE_1D, 0);
		glDisable(GL_COLOR_LOGIC_OP);
		glLogicOp(GL_COPY);
		glViewport(0,0,width,height);

		// G-BUFFER: render scene
        gbufferPass
        -> clear(0.0f, 0.0f, 0.0f, 0.0f)
        -> update("view",       view)
        -> update("projection", projection)
        -> update("red",        red)
        -> update("green",      green)
        -> update("blue",       blue)
        -> update("alpha",      1.0f);

        for ( unsigned int i = 0; i < model.size(); i++ )
		{
			gbufferPass
					->update( "model", model[i] )
					->run();
		}

        // bind accumulated bitmask & slicemap
   		glActiveTexture(GL_TEXTURE0 + 5);
    	glBindTexture(GL_TEXTURE_1D, acc_bitmask);
   		glActiveTexture(GL_TEXTURE0 + 6);
    	glBindTexture(GL_TEXTURE_2D, slicemappingPass->get("slice0_127"));
   		glActiveTexture(GL_TEXTURE0);

        // COMPOSITION: perform transmissive shadowmapping
		compositingPass
        ->clear(0,0,0,0)
        ->texture("positionMap", gbufferPass->get("positionOutput"))
        ->texture("colorMap",    gbufferPass->get("colorOutput"))
        ->update("lightView",        lightView)
        ->update("lightPerspective", lightProjection)
        ->update("view",             view)
        ->update("opacityPerSlice",  opacityPerSlice)
        ->run();

		// unbind textures
   		glActiveTexture(GL_TEXTURE0 + 5);
    	glBindTexture(GL_TEXTURE_1D, 0);
   		glActiveTexture(GL_TEXTURE0 + 6);
    	glBindTexture(GL_TEXTURE_2D, 0);
   		glActiveTexture(GL_TEXTURE0);

    });
}
