#include "ShaderTools/DefaultRenderLoop.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"

#include "VoxelizationTools/SlicemapUtilities.h"
#include "VoxelizationTools/OpenGLTestUtilities.h"

#define PI 3.14159265359f

// DESCRIPTION
/* This program uses a slicemap similarly to a shadowmap to simulate partial light absorption (i.e. foliage, transmissive material)
 * A 128-slices slicemap is used as the render target of the light source's view (see Slicemapping program).
 * A G-Buffer (color, position) is used as the render target of the camera view.
 * A screenfilling quad is rendered to evaluate the shadow map, by counting the set bits between the fragment's position and the light source.
 */

// SHADER PROGRAMS
auto sp = new ShaderProgram({"/Voxelization/simpleVertex.vert", "/Voxelization/gbuffer.frag"});
auto slicemappingShader = new ShaderProgram({"/Voxelization/simpleVertex.vert", "/Voxelization/sliceMap.frag"});
auto transmissiveShadowmapping	= new ShaderProgram({"/Voxelization/screenfill.vert", "/Voxelization/shadowmappingSliceMap.frag"});

// OBJECTS
auto cube = new Cube();
auto quad = new Quad();

// RENDERPASSES
int gridResX = 512;	// slice map resolution
int gridResY = 512;

auto pass = new RenderPass( cube, sp, width, height );	// render object
auto slicemappingPass = new SlicemapRenderPass( cube, slicemappingShader, gridResX, gridResY ); // render slice map
auto transmissiveShadowmappingPass = new RenderPass( quad, transmissiveShadowmapping);	// apply transmissive shadowmap

// GLOBAL VARIABLES
float red = 0.133f;   // object colors
float green = 0.545f;
float blue = 0.133f;

float opacityPerSlice = 0.15f;

// camera / object parameters
float near = 0.1f;
float far  = 4.0f;
glm::mat4 model = glm::mat4(1.0f);
std::vector<glm::mat4> positions;
glm::mat4 view = glm::lookAt(glm::vec3(0.0f,0.0f,1.5f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f) );
glm::mat4 projection = glm::perspective(60.0f * PI / 180.0f, (float) width/height, near, far );

// voxelization camera parameters
float lightNear = 0.0f;
float lightFar = 2.0f;
glm::mat4 lightView = glm::lookAt(glm::vec3(0.0f,1.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,-1.0f) );
glm::mat4 lightProjection = glm::ortho( -1.5f, 1.5f, -1.5f, 1.5f, lightNear, lightFar );

GLuint bitmask = createRGBA32UIBitMask();	// handle of 1D bitmask texture
GLuint acc_bitmask = createRGBA32UIAccBitMask();	// handle of 1D accumulated bitmask texture for value truncation
GLuint clearSlicemap[4] = {0, 0, 0, 0};

int main(int argc, char *argv[]) {

	// generate random small cube positions and a ground cube
	srand(time(NULL));
	for (int i = 0; i < 75; i++)
	{
		float offX = ( (float( rand()) / float(RAND_MAX) ) - 0.5f ); // -0.5..0.5
		float offY = ( (float( rand()) / float(RAND_MAX) ) * 0.75f - 0.25f) ; // 0.0..0.75
		float offZ = ( (float( rand()) / float(RAND_MAX) ) - 0.5f ); // -0.5..0.5
		positions.push_back( glm::translate(offX,offY,offZ) * glm::scale(glm::mat4(1.0f), 0.1f,0.005f,0.1f) ); // small plane
	}
	positions.push_back( glm::translate(0.0f,-0.5f,0.0f) * glm::scale(glm::mat4(1.0f), 1.25f, 0.01f, 1.25f ) ); // ground plane

    sp -> printUniformInfo();
    sp -> printInputInfo();
    sp -> printOutputInfo();

    slicemappingShader->printUniformInfo();
    slicemappingShader->printInputInfo();
    slicemappingShader->printOutputInfo();

    transmissiveShadowmapping->printUniformInfo();
    transmissiveShadowmapping->printInputInfo();
    transmissiveShadowmapping->printOutputInfo();

    renderLoop([]{
		if (glfwGetKey(window, GLFW_KEY_UP) 	== GLFW_PRESS) {view = glm::translate(glm::vec3(0.0f,0.0f,0.01f)) * view;}
		if (glfwGetKey(window, GLFW_KEY_DOWN) 	== GLFW_PRESS) {view = glm::translate(glm::vec3(0.0f,0.0f,-0.01f)) * view;}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) 	== GLFW_PRESS) {view = glm::translate(glm::vec3(-0.01f, 0.0f,0.0f)) * view;}
		if (glfwGetKey(window, GLFW_KEY_LEFT) 	== GLFW_PRESS) {view = glm::translate(glm::vec3(0.01f,0.0f,0.0f)) * view;}
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {glfwDestroyWindow(window); exit(-1);}
		if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {opacityPerSlice = glm::clamp( opacityPerSlice + 0.002f, 0.0f, 1.0f);}
		if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {opacityPerSlice = glm::clamp( opacityPerSlice - 0.002f, 0.0f, 1.0f);}

		// clear slicemap
		glBindFramebuffer( GL_FRAMEBUFFER, slicemappingPass->frameBufferObject->getFrameBufferObjectHandle());
		glClearBufferuiv(GL_COLOR, 0, clearSlicemap);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// enable bitwise logic OR operation on outgoing fragments and disable depth test
		glEnable(GL_COLOR_LOGIC_OP);
		glLogicOp(GL_OR);

		// bind bitmask to texture unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_1D, bitmask);

		// render slicemap
		slicemappingPass
		->update("view", lightView)
		->update("projection", lightProjection)
		->update("near", lightNear)
		->update("far", lightFar);

		//adjust viewport to slicemap resolution
		glViewport(0,0,gridResX,gridResY);

		for ( unsigned int i = 0; i < positions.size(); i++ )
		{
			slicemappingPass
					->update( "model", positions[i] )
					->run();
		}

		// restore default values
		glBindTexture(GL_TEXTURE_1D, 0);
		glDisable(GL_COLOR_LOGIC_OP);
		glLogicOp(GL_COPY);
		glViewport(0,0,width,height);

		// render scene
        pass
        -> clear(0.0f, 0.0f, 0.0f, 0.0f)
        -> update("view", view)
        -> update("projection", projection)
        -> update("red", red)
        -> update("green", green)
        -> update("blue", blue)
        -> update("alpha", 1.0f);

        for ( unsigned int i = 0; i < positions.size(); i++ )
		{
			pass
					->update( "model", positions[i] )
					->run();
		}

        // bind accumulated bitmask & slicemap to texture unit 5 & 6 (arbitrary units...)
   		glActiveTexture(GL_TEXTURE0 + 5);
    	glBindTexture(GL_TEXTURE_1D, acc_bitmask);
   		glActiveTexture(GL_TEXTURE0 + 6);
    	glBindTexture(GL_TEXTURE_2D, slicemappingPass->get("slice0_127"));
   		glActiveTexture(GL_TEXTURE0);

        // apply transmissive shadowmapping onto scene for visualization
		transmissiveShadowmappingPass
        ->clear(0,0,0,0)
        ->texture("positionMap", pass->get("positionOutput"))
        ->texture("colorMap", pass->get("colorOutput"))
        ->update("lightView", lightView)
        ->update("lightPerspective", lightProjection)
        ->update("view", view)
        ->update("opacityPerSlice", opacityPerSlice)
        ->run();

   		glActiveTexture(GL_TEXTURE0 + 5);
    	glBindTexture(GL_TEXTURE_1D, 0);
   		glActiveTexture(GL_TEXTURE0 + 6);
    	glBindTexture(GL_TEXTURE_2D, 0);
   		glActiveTexture(GL_TEXTURE0);

    });
}
