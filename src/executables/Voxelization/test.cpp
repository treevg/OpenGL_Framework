#include "ShaderTools/DefaultRenderLoop.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"

#include "SlicemapUtilities.h"

#define PI 3.14159265359f

/*TODO
 * - fix bugs
 * - enable slicemapping shader to use 128 slices
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

GLuint bitmask = 0;

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

void testZeros(GLuint texture, GLenum format = GL_RGBA)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	int width_, height_;
	glGetTexLevelParameteriv(GL_TEXTURE_2D,0, GL_TEXTURE_WIDTH, &width_);
	glGetTexLevelParameteriv(GL_TEXTURE_2D,0, GL_TEXTURE_HEIGHT, &height_);
//	std::cout<<"width: "<< width_ <<", height: "<< height_ << std::endl;

	unsigned int *data = (unsigned int*)malloc( sizeof(unsigned int) * height_ * width_ * 4);
	glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_INT, data);

	bool notzero = false;
	for( unsigned int i = 0; i < width_ * height_ * 4 ; i++ )
	{
		if ( data[i] > 0 )
		{
			std::cout<<"i[" << i / 4 << ", " << i % 4 << "] : " << data[i] << std::endl;
			notzero = true;
			break;
		}
	}
	if ( !notzero )
	{
		std::cout << "texture: " << texture <<" is all zeros..." << std::endl;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void testZeros1D(GLuint texture)
{
	glBindTexture(GL_TEXTURE_1D, texture);
	int width_ = 0; int height_ = 0 ;
	glGetTexLevelParameteriv(GL_TEXTURE_1D, 0, GL_TEXTURE_WIDTH, &width_);
//	std::cout<<"width: "<< width_ <<", height: "<< height_ << std::endl;

	unsigned int *data = (unsigned int*)malloc(sizeof(unsigned int) * width_ * 4);
	glGetTexImage(GL_TEXTURE_1D, 0, GL_RGBA_INTEGER, GL_UNSIGNED_INT, data);

	bool notzero = false;
	for( unsigned int i = 0; i < width_*4; i++ )
	{
		if ( data[i] > 0 )
		{
			std::cout<<"i[" << i / 4 << ", " << i % 4 <<"] : " << data[i] << std::endl;
			notzero = true;
		}
	}
	if ( !notzero )
	{
		std::cout << "1D texture: " << texture <<" is all zeros..." << std::endl;
	}

	std::cout << "i'm done with this crap " << std::endl;

	glBindTexture(GL_TEXTURE_1D, 0);
}

bool once = false;

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

    bitmask = createRGBA32UIBitMask();

	glEnable(GL_TEXTURE_1D);

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
        -> run();

//		testZeros(pass->get("fragmentColor"), GL_RGBA);

        if ( !once )
        {

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_LOGIC_OP);
        glLogicOp(GL_OR);

        // bind bitmask to image unit 0
		glBindImageTexture(0,           // image unit binding
		bitmask,  						// texture
		0,								// texture level
		GL_FALSE,                       // layered
		0,                              // layer
		GL_READ_ONLY,                   // access
		GL_RGBA32UI                     // format
		);

		// render slicemap
		slicemappingPass
		->clear(0,0,0,0)
        ->update("model", model)
        ->update("view", view)
        ->update("projection", projection)
		->update("near", near)
		->update("far", far)
//		->update("numSlicemaps", numSlicemaps)
		->run();

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_LOGIC_OP);
		glLogicOp(GL_COPY);

//    	once = true;

    	}

    // DEBUG
		testZeros(slicemappingPass->get("slice0_127"), GL_RGBA_INTEGER);

        // bind slicemap to image unit 0
        glUseProgram( projectSlicemap->getProgramHandle() );

        glBindImageTexture(1,           // image unit binding
		slicemappingPass->get("slice0_127"), // texture
		0,								// texture level
		GL_FALSE,                       // layered
		0,                              // layer
		GL_READ_ONLY,                   // access
		GL_RGBA32UI                     // format
		);

		projectSlicemapPass
        ->clear(0,0,0,0)
        ->texture("baseTexture", pass->get("fragmentColor"))
        ->update("backgroundTransparency", backgroundTransparency)
        ->run();

    });
}
