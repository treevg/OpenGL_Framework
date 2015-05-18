
#include "demo_oculus.h"
#include <fstream>
#include <string>
#include <typeinfo>


using namespace glm;



GLFWwindow* window;

/* parameters */


float lum = 0.5;
float speed = 10.0f;


double lasttime;
vec3 lightPosition = vec3(200,200,200);
float shinines = 32.0f;
int width, height;





/*          MATRIX STUFF      */
//glm::mat4 projMat = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);

glm:: mat4 modelCastle(1);
glm:: mat4 skyModel(1);


/* GAME LOGIC*/
float castel_y = -8.0;



RenderPassModel* castlePass;

Camera* camera;
Model* castle;


vector<VertexArrayObject*> castleMeshes;



/* static methods */


static glm::mat4 getLookAt()
{

	mat4 view = camera->looksAt();

	return view;

}


static void log(ShaderProgram* s)
{

	s -> printUniformInfo();
	s -> printInputInfo();
	s -> printOutputInfo();

}

static void printMatrix(mat4 matrix)
{

	 std::cout<<"matrix:  "<<glm::to_string(matrix)<<std::endl;

}


static void quit()
{

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);

	//free all resources

}

/*input from keybord */

static void  moveWithKeybord(float deltaTime)
{

	
	quit();

}

/*input from mouse */
static void  lookAround()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	camera->rotateWithMouse(window, getWidth(window), getHeight(window));
}



void Demo_Oculus::setMatrix()
{

	modelCastle= glm::translate(modelCastle, glm::vec3(2,castel_y, 2));
//	modelCastle = glm::rotate(modelCastle, -29.0f, glm::vec3(0,1,0));
	modelCastle = glm::scale(modelCastle, glm::vec3(0.01,0.01,0.01));

	
	skyModel = translate(skyModel, vec3(-2.0, -12.0, -2.0));
	skyModel =  scale(skyModel, vec3(15, 15, 15 ));

	
}

void Demo_Oculus::init()
{
	/* Camera  */
	window = generateWindow();
	

	

//  shader

	auto model = new ShaderProgram("/Warpping/model.vert", "/Warpping/model.frag");
	auto demoSp= new ShaderProgram("/Warpping/demo.vert", "/Warpping/demo.frag");

	this->setMatrix();
	castle = new Model(RESOURCES_PATH "/castle01.obj",modelCastle, "model");




	/* Meshes */


	castleMeshes = castle->getMeshes();

	                
	castlePass = new RenderPassModel(castleMeshes, model);
        castlePass->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
		


}

Demo_Oculus::Demo_Oculus()
{


	init();
	renderSzene();
}	


Demo_Oculus::~Demo_Oculus()
{
	
	delete castle;
	
        delete castlePass;
	

}




void  Demo_Oculus::renderSzene()
{


	Cube* cube = new Cube();
	ShaderProgram* sp  = new ShaderProgram( "/Test_Telepresence/phong.vert", "/Test_Telepresence/phong.frag" );
	RenderPass* cubePass = new RenderPass(
		cube,
		sp);

	cubePass
		->update("diffuseColor", vec3(1.0, 0.0, 0.0))
		->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);


                 

	render(window, [&](float deltaTime, glm::mat4 projection, glm::mat4 view){



                  glfwGetWindowSize(window, &width, &height);


			castlePass
			->  update("uniformModel", modelCastle)
			->  update("uniformView", view)
			->  update("uniformProjection", projection)
			->  update("attenuatFactor", false)
			->  run();

		   cubePass
			->update("modelMatrix", translate(mat4(1), vec3(2.0f, 1.0f, -10.0f)))
			->update("projectionMatrix", projection)
			->update("viewMatrix", view)
		
			->run();

			
	});
    

}



int main(int argc, char *argv[])
{

	Demo_Oculus * g = new Demo_Oculus();

	delete g;
}
