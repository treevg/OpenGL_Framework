
#include "GameOculus.h"
#include <fstream>
#include <string>


using namespace glm;



GLFWwindow* window;

/* parameters */


float z=50.0;
float x=0.0;
float size = 0.5;
float lum = 0.5;
float speed = 8.0f;
bool targetReached = false;
bool warpping;
double lasttime;
vec3 lightPosition = vec3(200,200,200);
float shinines = 32.0f;
string path;
std::queue<glm::mat4> latencyQueue;
int latencyFrameCount = 6;

int width, height;



/*          MATRIX STUFF      */

glm:: mat4 modelCastle(1);
glm:: mat4 modelChest(1);
glm:: mat4 modelWindMill(1);
glm:: mat4 vikingModel(1);
glm:: mat4 vikingModel2(1);
glm:: mat4 followMeModel(1);
glm:: mat4 modelTree(1);
glm:: mat4 modelTree1(1);
glm:: mat4 modelTree2(1);
glm:: mat4 modelTree3(1);
glm:: mat4 modelTree4(1);
glm:: mat4 modelTree5(1);
glm:: mat4 modelTree6(1);
glm:: mat4 modelTree7(1);
glm:: mat4 modelTree8(1);
glm:: mat4 modelTree9(1);
glm:: mat4 skyModel(1);
glm:: mat4 modelTerra(1);
glm:: mat4 modelHouse1(1);
glm:: mat4 modelHouse2(1);
glm:: mat4 modelHouse3(1);
glm:: mat4 modelHouse4(1);
glm:: mat4 modelHouse5(1);
glm:: mat4 modelHouse6(1);
glm:: mat4 modelHouse8(1);
glm:: mat4 modelHouse9(1);
glm:: mat4 lowHouse1(1);
glm:: mat4 lowHouse2(1);
glm:: mat4 lowHouse3(1);
glm:: mat4 lowHouse4(1);
glm:: mat4 lowHouse5(1);
glm:: mat4 lowHouse6(1);
glm:: mat4 lowHouse7(1);
glm:: mat4 lowHouse8(1);
glm:: mat4 lowHouse9(1);
glm:: mat4 modelHouse7(1);
glm:: mat4 modelBush1(1);
glm:: mat4 modelBush2(1);
glm:: mat4 modelBush3(1);
glm:: mat4 modelBush4(1);
glm:: mat4 modelBush5(1);
glm:: mat4 modelBush6(1);
glm:: mat4 modelBush7(1);
glm:: mat4 modelBush8(1);
glm:: mat4 modelWell(1);
glm:: mat4 modelPolyTrees1(1);
glm:: mat4 modelPolyTrees2(1);
glm:: mat4 modelPolyTrees3(1);
glm:: mat4 modelPolyTrees4(1);
glm:: mat4 modelGrass1(1);
glm:: mat4 modelGrass2(1);
glm:: mat4 modelGrass3(1);
glm:: mat4 modelGrass4(1);
glm:: mat4 modelGrass5(1);
glm:: mat4 modelGrass6(1);
glm:: mat4 modelGrass7(1);
glm:: mat4 modelGrass8(1);
glm:: mat4 modelGrass9(1);
glm:: mat4 modelGrass10(1);
glm:: mat4 modelGrass11(1);
glm:: mat4 modelGrass12(1);
glm:: mat4 modelGrass16(1);
glm:: mat4 modelGrass15(1);
glm:: mat4 modelGrass17(1);
glm:: mat4 modelGrass18(1);
glm:: mat4 modelGrass19(1);
glm:: mat4 modelGrass13(1);
glm:: mat4 modelGrass14(1);
glm:: mat4 modelGrass20(1);
glm:: mat4 modelGrass21(1);
glm:: mat4 modelGrass22(1);
glm:: mat4 modelGrass23(1);
glm:: mat4 modelGrass24(1);
glm:: mat4 modelGrass25(1);
glm:: mat4 modelGrass26(1);
glm:: mat4 modelGrass27(1);
glm:: mat4 modelGrass28(1);
glm:: mat4 modelCart(1);
glm:: mat4 modelPig(1);
glm:: mat4 modelSmaug(1);


/* GAME LOGIC*/
float castel_y = -6.2;
vec3 currentChestPosition;





RenderPassModel*  skyBoxPass;

RenderPass*  plane ;
RenderPassModel* trees;
RenderPass* diffWarp;
RenderPassModel* castlePass;
RenderPassModel* chestPass;
RenderPassModel* housePass;
RenderPassModel* house2Pass;
RenderPassModel* house3Pass;
RenderPassModel* lowPollyPass;
RenderPassModel* windMillPass;
RenderPassModel* vikingPass;
RenderPassModel* terraPass;
RenderPassModel* bushPass;
RenderPassModel* wellPass;
RenderPassModel* grassPass;
RenderPassModel* lowPollyTreesPass;
RenderPassModel* cartPass;
RenderPassModel* pigPass;
RenderPassModel* smaugPass;
RenderPassCollector* collector;



Camera* camera;


Model* myModel;
Model* bush;
Model* castle;
Model* chest;
Model* windMill;
Model* viking;
Model* viking2;
Model* terraModel;
Model* skyDom;
Model* house1;
Model* house2;
Model* house3;
Model* lowPollyHouse;
Model* wellModel;
Model* lowPollyTreesModel;
Model* grassModel;
Model* cartModel;
Model* pigModel;
Model* smaugModel;


vector<VertexArrayObject*> meshes;
vector<VertexArrayObject*> castleMeshes;
vector<VertexArrayObject*> chestMeshes;
vector<VertexArrayObject*> windMillMeshes;
vector<VertexArrayObject*> vikingMeshes;
vector<VertexArrayObject*> vikingMeshes2;
vector<VertexArrayObject*> skyMeshes;
vector<VertexArrayObject*> terraMeshes;
vector<VertexArrayObject*> vaos;
vector<VertexArrayObject*> house1Meshes;
vector<VertexArrayObject*> house2Meshes;
vector<VertexArrayObject*> house3Meshes;
vector<VertexArrayObject*> lowPollyHouseMeshes;
vector<VertexArrayObject*> bushMeshes;
vector<VertexArrayObject*> wellMeshes;
vector<VertexArrayObject*> polyTreesMeshes;
vector<VertexArrayObject*> grassMeshes;
vector<VertexArrayObject*> cartMeshes;
vector<VertexArrayObject*> pigMeshes;
vector<VertexArrayObject*> smaugMeshes;

std::map<string,ShaderProgram*> shaderPrograms;




GLuint textureHandle;


//holefilling

int numMipmaps = glm::log2(glm::max<float>(getWidth(window), getHeight(window))) * 2;
Quad* quad;

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

	// std::cout<<"matrix:  "<<glm::to_string(matrix)<<std::endl;

}


static void quit()
{

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);

	//free all resources

}

/*input from keybord */

static void  moveWithKeybord(float deltaTime)
{

	camera->moveWithKey(window, speed*deltaTime);
	quit();

}

/*input from mouse */
static void  lookAround()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	camera->rotateWithMouse(window, getWidth(window), getHeight(window));
}





static void simulateLanetcy(int frameCount, glm::mat4 viewMat)
{

	latencyQueue.push(viewMat);

	if (latencyQueue.size() > frameCount)
	{

		latencyQueue.pop();
	}
}




void GameOculus::setMatrix()
{

	modelCastle= glm::translate(modelCastle, glm::vec3(50,castel_y, 120));
	modelCastle = glm::rotate(modelCastle, -29.0f, glm::vec3(0,1,0));
	modelCastle = glm::scale(modelCastle, glm::vec3(0.005,0.005,0.005));

	modelChest = translate(modelChest, currentChestPosition);
	modelChest = glm::scale(modelChest, glm::vec3(0.8,0.8,0.8));

	modelWindMill = translate(modelWindMill, vec3(116.981, -2.1, 144.5));
	modelWindMill = rotate(modelWindMill, 24.0f, vec3(0,1,0));
	modelWindMill = scale(modelWindMill, vec3(0.005, 0.005, 0.005));

	vikingModel = translate(vikingModel,vec3(100, -6.05, 151) );
	vikingModel = glm::rotate(vikingModel, 19.0f, glm::vec3(0,1,0));
	vikingModel = scale(vikingModel, vec3(0.3, 0.3, 0.3));

	vikingModel2 = translate(vikingModel2,vec3(94, -6.05, 157) );
	vikingModel2 = glm::rotate(vikingModel2, -30.0f, glm::vec3(0,1,0));
	vikingModel2 = scale(vikingModel2, vec3(0.3, 0.3, 0.3));


	skyModel = translate(skyModel, vec3(-2.0, -12.0, -2.0));
	skyModel =  scale(skyModel, vec3(15, 15, 15 ));
	

	modelTerra = translate(modelTerra, vec3(52.0, -6.0, 50.0));
	modelTerra =  scale(modelTerra, vec3(100, 200, 150));

	modelHouse1 = translate(modelHouse1, vec3(95.0, -2.8001, 189.0));
	modelHouse1 = glm::rotate(modelHouse1, -29.0f, glm::vec3(0,1,0));
	modelHouse1 =  scale(modelHouse1, vec3(1.8, 1.8, 1.8));

	modelHouse2 = translate(modelHouse2, vec3(110.0, -4.8001, 198.0));
	modelHouse2 = glm::rotate(modelHouse2, -29.0f, glm::vec3(0,1,0));
	modelHouse2 =  scale(modelHouse2, vec3(1.2, 1.2, 1.2));

	modelHouse3 = translate(modelHouse3, vec3(80.0, -6.001, 208.0));
	modelHouse3 = glm::rotate(modelHouse3, 45.0f, glm::vec3(0,1,0));
	modelHouse3 =  scale(modelHouse3, vec3(0.02, 0.02, 0.02));

	modelHouse4 = translate(modelHouse4, vec3(95.0, -6.001, 220.0));
	modelHouse4 = glm::rotate(modelHouse4, 45.0f, glm::vec3(0,1,0));
	modelHouse4 =  scale(modelHouse4, vec3(0.02, 0.02, 0.02));

	modelHouse5 = translate(modelHouse5, vec3(111.0, -6.001, 232.0));
	modelHouse5 = glm::rotate(modelHouse5, 45.0f, glm::vec3(0,1,0));
	modelHouse5 =  scale(modelHouse5, vec3(0.02, 0.02, 0.02));

	modelHouse8 = translate(modelHouse8, vec3(157.0, -6.001, 184.10));
	modelHouse8 = glm::rotate(modelHouse8, -0.50f, glm::vec3(0,1,0));
	modelHouse8 =  scale(modelHouse8, vec3(0.017, 0.017, 0.017));


	modelHouse9 = translate(modelHouse9, vec3(160.0, -2.8001, 161.0));
	modelHouse9 = glm::rotate(modelHouse9, -13.80f, glm::vec3(0,1,0));
	modelHouse9 =  scale(modelHouse9, vec3(1.8, 1.8, 1.8));

	lowHouse1  = translate(lowHouse1, vec3(144.9, -5.201, 136.0));
	lowHouse1 = glm::rotate(lowHouse1, 45.0f, glm::vec3(0,1,0));
	lowHouse1 =  scale(lowHouse1, vec3(3, 3,  3));

	lowHouse2  = translate(lowHouse2, vec3(159.0, -5.201, 120.0));
	lowHouse2 = glm::rotate(lowHouse2, 45.0f, glm::vec3(0,1,0));
	lowHouse2 =  scale(lowHouse2, vec3(3, 3,  3));

	lowHouse3  = translate(lowHouse3, vec3(160.0, -5.201, 146.0));
	lowHouse3 = glm::rotate(lowHouse3, 45.0f, glm::vec3(0,1,0));
	lowHouse3 =  scale(lowHouse3, vec3(3, 3,  3));

	lowHouse4  = translate(lowHouse4, vec3(171.0, -5.201, 128.0));
	lowHouse4 = glm::rotate(lowHouse4, 45.0f, glm::vec3(0,1,0));
	lowHouse4 =  scale(lowHouse4, vec3(3, 3,  3));

	lowHouse5  = translate(lowHouse5, vec3(123.0, -5.201, 102.0));
	lowHouse5 = glm::rotate(lowHouse5, 45.0f, glm::vec3(0,1,0));
	lowHouse5 =  scale(lowHouse5, vec3(3, 3,  3));

	lowHouse6  = translate(lowHouse6, vec3(173.0, -5.201, 180.0));
	lowHouse6 = glm::rotate(lowHouse6, 47.0f, glm::vec3(0,1,0));
	lowHouse6 =  scale(lowHouse6, vec3(3, 3,  3));

	lowHouse7  = translate(lowHouse7, vec3(130.0, -5.201, 127.8));
	lowHouse7 = glm::rotate(lowHouse7, 45.0f, glm::vec3(0,1,0));
	lowHouse7 =  scale(lowHouse7, vec3(3, 3,  3));

	lowHouse8  = translate(lowHouse8, vec3(136.0, -5.201, 108.0));
	lowHouse8 = glm::rotate(lowHouse8, 45.0f, glm::vec3(0,1,0));
	lowHouse8 =  scale(lowHouse8, vec3(3, 3,  3));

	lowHouse9  = translate(lowHouse9, vec3(148.0, -5.201, 115.0));
	lowHouse9 = glm::rotate(lowHouse9, 45.0f, glm::vec3(0,1,0));
	lowHouse9 =  scale(lowHouse9, vec3(3, 3,  3));

	modelHouse6 = translate(modelHouse6, vec3(131.0, -2.8001, 215.0));
	modelHouse6 = glm::rotate(modelHouse6, -29.0f, glm::vec3(0,1,0));
	modelHouse6 =  scale(modelHouse6, vec3(1.8, 1.8, 1.8));

	modelHouse7 = translate(modelHouse7, vec3(147.0, -4.8001, 206.80));
	modelHouse7 = glm::rotate(modelHouse7, 29.3f, glm::vec3(0,1,0));
	modelHouse7 =  scale(modelHouse7, vec3(1.2, 1.2, 1.2));

	modelBush1 = translate(modelBush1, vec3(105.0, -6.0, 128.0));
	//    modelBush1 =  scale(modelBush1, vec3(0.5, 0.5, 0.5));

	modelBush2 = translate(modelBush2, vec3(101.0, -6.0, 140.0));
	// modelBush2 =  scale(modelBush2, vec3(0.7, 0.7, 0.7));

	modelBush3 = translate(modelBush3, vec3(115, -6.0, 117.38));
	//modelBush3 =  scale(modelBush3, vec3(0.5, 0.5, 0.5));

	modelBush4 = translate(modelBush4, vec3(101.2, -6.0, 132.3));
	// modelBush4 =  scale(modelBush4, vec3(0.9, 0.9, 0.9));

	modelBush5 = translate(modelBush5, vec3(87.3, -6.0, 175.73));
	//   modelBush5 =  scale(modelBush5, vec3(0.7, 0.7, 0.7));

	modelBush6 = translate(modelBush6, vec3(79.9, -6.0, 157.73));
	//     modelBush6 =  scale(modelBush6, vec3(0.8, 0.85, 0.9));

	modelBush7 = translate(modelBush7, vec3(109.6, -6.0, 124.3));

	modelWell = translate(modelWell, vec3(88, -6.0, 168.73));
	modelWell =  scale(modelWell, vec3(0.01, 0.01, 0.01));


	modelTree = translate(modelTree, vec3(112.41, -6.0, 122.73));
	modelTree =  scale(modelTree, vec3(0.5, 0.5, 0.5));


	modelTree1= translate(modelTree1, vec3(109, -6.0, 102.73));
	modelTree1 =  scale(modelTree1, vec3(0.5, 0.5, 0.5));

	modelTree2 = translate(modelTree2, vec3(98.49, -6.0, 93.97));
	modelTree2 =  scale(modelTree2, vec3(0.5, 0.5, 0.5));

	modelTree3 = translate(modelTree3, vec3(160.49, -6.0, 195));
	modelTree3 =  scale(modelTree3, vec3(0.5, 0.5, 0.5));
	modelTree4 = translate(modelTree4, vec3(149.49, -6.0, 222.73));
	modelTree4 =  scale(modelTree4, vec3(0.5, 0.5, 0.5));
	modelTree5  = translate(modelTree5, vec3(140.49, -6.0, 157.73));
	modelTree5 =  scale(modelTree5, vec3(0.5, 0.5, 0.5));

	modelTree6 = translate(modelTree6, vec3(180.49, -6.0, 155.73));
	modelTree6 =  scale(modelTree6, vec3(0.5, 0.5, 0.5));

	modelTree7 = translate(modelTree7, vec3(190.49, -6.0, 161.73));
	modelTree7 =  scale(modelTree7, vec3(0.5, 0.5, 0.5));

	modelTree8 = translate(modelTree8, vec3(69.49, -6.0, 169.73));
	modelTree8 =  scale(modelTree8, vec3(0.5, 0.5, 0.5));

	modelTree9 = translate(modelTree9, vec3(75.49, -6.0, 182.73));
	modelTree9 =  scale(modelTree9, vec3(0.5, 0.5, 0.5));


	modelPolyTrees1 = translate(modelPolyTrees1, vec3(114.49, -5.0, 87.73));
	modelPolyTrees1 =  scale(modelPolyTrees1, vec3(0.001, 0.001, 0.001));

	modelPolyTrees2 = translate(modelPolyTrees2, vec3(176.49, -5.0, 152.73));
	modelPolyTrees2 =  scale(modelPolyTrees2, vec3(0.001, 0.001, 0.001));

	modelPolyTrees3 = translate(modelPolyTrees3, vec3(57.49, -5.0, 188.73));
	modelPolyTrees3 =  scale(modelPolyTrees3, vec3(0.001, 0.001, 0.001));

	modelPolyTrees4 = translate(modelPolyTrees4, vec3(148.49, -5.0, 223.73));
	modelPolyTrees4 =  scale(modelPolyTrees4, vec3(0.001, 0.001, 0.001));

	modelGrass1 = translate(modelGrass1, vec3(145.49, -6.0, 195.73));
	//    modelGrass1 =  scale(modelGrass1, vec3(0.001, 0.001, 0.001));
	modelGrass2 = translate(modelGrass2, vec3(121.2, -6.0, 142.73));
	modelGrass3 = translate(modelGrass3, vec3(138.49, -6.0, 149.73));
	modelGrass21 = translate(modelGrass21, vec3(142.8, -6.0, 141.73));
	modelGrass4 = translate(modelGrass4, vec3(91.48, -6.0, 159.89));
	modelGrass5 = translate(modelGrass5, vec3(102.218, -6.0, 146.13));
	modelGrass6 = translate(modelGrass6, vec3(104.34, -6.0, 193.73));
	modelGrass7 = translate(modelGrass7, vec3(100.89, -6.0, 190.83));
	modelGrass8 = translate(modelGrass8, vec3(98.11, -6.0,  177.6));
	modelGrass9 = translate(modelGrass9, vec3(101.58, -6.0, 166.03));
	modelGrass10 = translate(modelGrass10, vec3(111.88, -6.0, 152.05));
	modelGrass11 = translate(modelGrass11, vec3(119.8, -6.0, 156.73));
	modelGrass12 = translate(modelGrass12, vec3(127.3, -6.0, 156.73));
	modelGrass13 = translate(modelGrass13, vec3(138.49, -6.0, 175.73));
	modelGrass22 = translate(modelGrass22, vec3(125.8, -6.0, 180.73));
	modelGrass14 = translate(modelGrass14, vec3(109.48, -6.0, 181.1));
	modelGrass15 = translate(modelGrass15, vec3(132.218, -6.0, 178.13));
	modelGrass16 = translate(modelGrass16, vec3(131.34, -6.0, 152.3));
	modelGrass17 = translate(modelGrass17, vec3(118.89, -6.0, 169.83));
	modelGrass18 = translate(modelGrass18, vec3(119.11, -6.0,  182.6));
	modelGrass19 = translate(modelGrass19, vec3(126.58, -6.0, 193.03));
	modelGrass20 = translate(modelGrass20, vec3(115.88, -6.0, 162.05));
	modelGrass23 = translate(modelGrass23, vec3(179.218, -6.0, 194.13));
	modelGrass24 = translate(modelGrass24, vec3(108.34, -6.0, 174.3));
	modelGrass25 = translate(modelGrass25, vec3(139.89, -6.0, 165.83));
	modelGrass26 = translate(modelGrass26, vec3(137.11, -6.0,  185.6));
	modelGrass27 = translate(modelGrass27, vec3(134.58, -6.0, 159.03));
	modelGrass28 = translate(modelGrass28, vec3(115.88, -6.0, 198.05));

	modelCart = translate(modelCart, vec3(97.88, -6.0, 168.05));
	modelCart = glm::rotate(modelCart, 55.0f, glm::vec3(1,0,0));
	modelCart =  scale(modelCart, vec3(0.015, 0.015, 0.015));

	modelPig = translate(modelPig, vec3(115.88, -6.0, 150.05));
	modelPig =  scale(modelPig, vec3(0.3, 0.3, 0.3));
        modelSmaug =  translate(modelSmaug, vec3(152.48, 0.0, 99.08));
  	modelSmaug = glm::rotate(modelSmaug, 0.45f, glm::vec3(1,0,0));



}

void GameOculus::init()
{
	/* Camera  */
	window = generateWindow();
	camera =  new Camera();

	auto  quad = new Quad();

	auto grid = new Grid(getWidth(window), getHeight(window));


	//Warping shader
	auto warp = new ShaderProgram("/Warpping/warp.vert", "/Raytracing/warp.frag");

// other shader
	auto sp = new ShaderProgram("/Test_ShaderTools/test.vert", "/Test_ShaderTools/test.frag");
	auto sp1 = new ShaderProgram("/Warpping/myTest.vert", "/Warpping/myTest.frag");
	auto model = new ShaderProgram("/Warpping/model.vert", "/Warpping/model.frag");
	auto demoSp= new ShaderProgram("/Warpping/demo.vert", "/Warpping/demo.frag");
	auto suzanneSp = new ShaderProgram("/Warpping/suzanne.vert", "/Warpping/suzanne.frag");

  
        currentChestPosition = vec3(60.43, -6, 147.216);
  

	this->setMatrix();


	/* Models */
	string modSP = "model";
	string demo = "demo";


	
	myModel = new Model(RESOURCES_PATH "/Tree.obj", modelTree, demo);
	castle = new Model(RESOURCES_PATH "/castle01.obj",modelCastle, modSP);
	chest = new Model(RESOURCES_PATH "/chest.obj", modelChest, modSP);
	windMill = new Model(RESOURCES_PATH "/windmill02.obj", modelWindMill, modSP);
	viking = new Model(RESOURCES_PATH "/viking.obj", vikingModel, "suzane");
	viking2 = new Model(RESOURCES_PATH "/viking.obj", vikingModel2, "suzane");
	skyDom = new Model(RESOURCES_PATH "/sfere1.obj", skyModel, demo);
	terraModel = new Model(RESOURCES_PATH "/dempls.obj", modelTerra, modSP);
	house1 = new Model(RESOURCES_PATH "/casaMedieval.obj", modelHouse1, modSP);
	house2 = new Model(RESOURCES_PATH "/House1Upload.obj", modelHouse2, modSP);
	house3 = new Model(RESOURCES_PATH "/MedievalHouse1.obj", modelHouse3, modSP);
	lowPollyHouse = new Model(RESOURCES_PATH "/lowPolyHouse.obj", lowHouse1, modSP);
	bush  = new Model(RESOURCES_PATH "/Bush1.obj", modelBush1, demo);
	wellModel =  new Model(RESOURCES_PATH "/well.obj", modelWell, modSP);
	lowPollyTreesModel = new Model(RESOURCES_PATH "/trees lo-poly.obj", modelPolyTrees1, demo);
	grassModel = new Model (RESOURCES_PATH "/Grass_01.obj", modelGrass1, modSP);
	cartModel = new Model (RESOURCES_PATH "/wagen1_Lp_End.3ds", modelCart, modSP);
	pigModel = new Model (RESOURCES_PATH "/pig.obj", modelPig, modSP);
	smaugModel = new Model (RESOURCES_PATH "/smaug.obj", modelSmaug, modSP);

	/* external Textures */



	/* Meshes */

	meshes = myModel->getMeshes();
	castleMeshes = castle->getMeshes();
	chestMeshes  = chest->getMeshes();
	windMillMeshes = windMill->getMeshes();
	vikingMeshes = viking->getMeshes();
	vikingMeshes2 = viking2->getMeshes();
	skyMeshes = skyDom->getMeshes();
	terraMeshes = terraModel->getMeshes();
	house1Meshes = house1->getMeshes();
	house2Meshes = house2->getMeshes();
	house3Meshes = house3->getMeshes();
	lowPollyHouseMeshes = lowPollyHouse->getMeshes();
	bushMeshes = bush ->getMeshes();
	wellMeshes = wellModel->getMeshes();
	polyTreesMeshes = lowPollyTreesModel->getMeshes();
	grassMeshes = grassModel->getMeshes();
	cartMeshes = cartModel->getMeshes();
	pigMeshes = pigModel->getMeshes();
	smaugMeshes = smaugModel->getMeshes();


	shaderPrograms["model"] = model;
	shaderPrograms["suzane"] = suzanneSp;
	shaderPrograms["test"] = sp;
	shaderPrograms["demo"] = demoSp;

	collector = new RenderPassCollector(vaos, shaderPrograms, getWidth(window), getHeight(window));
	//collector->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
    





	/* Render Passes */
	if (warpping)
	{


	
		/**GET ALL TO COLLECTOR */

		collector->addVAOS(castleMeshes);
		collector->addVAOS(windMillMeshes);
		collector->addVAOS(vikingMeshes);
		collector->addVAOS(vikingMeshes2);
		collector->addVAOS(skyMeshes);
		collector->addVAOS(chestMeshes);
		collector->addVAOS(terraMeshes);
		collector->addVAOS(house1Meshes);
		collector->addVAOS(house1->setModelMatrix(modelHouse6));
		collector->addVAOS(house2Meshes);
		collector->addVAOS(house2->setModelMatrix(modelHouse7));
		collector->addVAOS(house3Meshes);
    collector->addVAOS(house3->setModelMatrix(modelHouse4));
    collector->addVAOS(house3->setModelMatrix(modelHouse5));
    collector->addVAOS(house3->setModelMatrix(modelHouse8));
    collector->addVAOS(house1->setModelMatrix(modelHouse9));
    collector->addVAOS(lowPollyHouseMeshes);
    collector->addVAOS(lowPollyHouse->setModelMatrix(lowHouse2));
    collector->addVAOS(lowPollyHouse->setModelMatrix(lowHouse3));
    collector->addVAOS(lowPollyHouse->setModelMatrix(lowHouse4));
    collector->addVAOS(lowPollyHouse->setModelMatrix(lowHouse5));
    collector->addVAOS(lowPollyHouse->setModelMatrix(lowHouse6));
    collector->addVAOS(lowPollyHouse->setModelMatrix(lowHouse7));
    collector->addVAOS(lowPollyHouse->setModelMatrix(lowHouse8));
    collector->addVAOS(lowPollyHouse->setModelMatrix(lowHouse9));
    collector->addVAOS(bushMeshes);
    collector->addVAOS(bush->setModelMatrix(modelBush2));
    collector->addVAOS(bush->setModelMatrix(modelBush3));
    collector->addVAOS(bush->setModelMatrix(modelBush4));
    collector->addVAOS(bush->setModelMatrix(modelBush5));
    collector->addVAOS(bush->setModelMatrix(modelBush6));
    collector->addVAOS(bush->setModelMatrix(modelBush7));
    collector->addVAOS(wellMeshes);
    collector->addVAOS(polyTreesMeshes);
    collector->addVAOS(lowPollyTreesModel->setModelMatrix(modelPolyTrees2));
    collector->addVAOS(lowPollyTreesModel->setModelMatrix(modelPolyTrees3));
    collector->addVAOS(meshes);
    collector->addVAOS(myModel->setModelMatrix(modelTree1));
    collector->addVAOS(myModel->setModelMatrix(modelTree2));
    collector->addVAOS(myModel->setModelMatrix(modelTree3));
    collector->addVAOS(myModel->setModelMatrix(modelTree4));
    collector->addVAOS(myModel->setModelMatrix(modelTree5));
    collector->addVAOS(myModel->setModelMatrix(modelTree6));
    collector->addVAOS(myModel->setModelMatrix(modelTree7));
    collector->addVAOS(myModel->setModelMatrix(modelTree8));
    collector->addVAOS(myModel->setModelMatrix(modelTree9));
    collector->addVAOS(grassMeshes);
    collector->addVAOS(grassModel->setModelMatrix(modelGrass2));
    collector->addVAOS(grassModel->setModelMatrix(modelGrass3));
    collector->addVAOS(grassModel->setModelMatrix(modelGrass4));
    collector->addVAOS(grassModel->setModelMatrix(modelGrass5));
    collector->addVAOS(grassModel->setModelMatrix(modelGrass6));
    collector->addVAOS(grassModel->setModelMatrix(modelGrass7));
    collector->addVAOS(grassModel->setModelMatrix(modelGrass8));
    collector->addVAOS(grassModel->setModelMatrix(modelGrass9));
    collector->addVAOS(grassModel->setModelMatrix(modelGrass10));
    collector->addVAOS(grassModel->setModelMatrix(modelGrass11));
    collector->addVAOS(grassModel->setModelMatrix(modelGrass12));
    collector->addVAOS(grassModel->setModelMatrix(modelGrass13));
    collector->addVAOS(grassModel->setModelMatrix(modelGrass14));
    collector->addVAOS(grassModel->setModelMatrix(modelGrass15));
    collector->addVAOS(grassModel->setModelMatrix(modelGrass16));
    collector->addVAOS(grassModel->setModelMatrix(modelGrass17));
    collector->addVAOS(grassModel->setModelMatrix(modelGrass18));
    collector->addVAOS(grassModel->setModelMatrix(modelGrass19));
    collector->addVAOS(grassModel->setModelMatrix(modelGrass20));
    collector->addVAOS(grassModel->setModelMatrix(modelGrass21));
    collector->addVAOS(grassModel->setModelMatrix(modelGrass22));
    collector->addVAOS(grassModel->setModelMatrix(modelGrass23));
    collector->addVAOS(grassModel->setModelMatrix(modelGrass24));
    collector->addVAOS(grassModel->setModelMatrix(modelGrass25));
    collector->addVAOS(grassModel->setModelMatrix(modelGrass26));
    collector->addVAOS(grassModel->setModelMatrix(modelGrass27));
    collector->addVAOS(grassModel->setModelMatrix(modelGrass28));
    collector->addVAOS(pigMeshes);
    collector->addVAOS(cartMeshes);
    collector->addVAOS(smaugMeshes);
    collector->addVAOS(lowPollyTreesModel->setModelMatrix(modelPolyTrees4));




   

    
    diffWarp = new RenderPass(grid, warp, getWidth(window), getHeight(window));
//    diffWarp->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);


	}
	else
	{

		trees = new RenderPassModel(meshes, model);
                trees->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
		castlePass = new RenderPassModel(castleMeshes, model );
		castlePass->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
		chestPass = new RenderPassModel(chestMeshes, model );
		chestPass->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
		windMillPass = new RenderPassModel(windMillMeshes, model );
		windMillPass->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
		vikingPass =  new RenderPassModel(vikingMeshes, suzanneSp );
		vikingPass->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
		skyBoxPass = new RenderPassModel(skyMeshes, demoSp);
                skyBoxPass->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);

		terraPass = new RenderPassModel(terraMeshes, model);
		terraPass->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
		housePass = new RenderPassModel(house1Meshes, model);
		housePass->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
		house2Pass = new RenderPassModel(house2Meshes, model);
		house2Pass->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
		house3Pass = new RenderPassModel(house3Meshes, model);
		house3Pass->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
		lowPollyPass = new RenderPassModel(lowPollyHouseMeshes, model);
		lowPollyPass->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
		bushPass = new RenderPassModel(bushMeshes, demoSp);
		bushPass->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
		wellPass  = new RenderPassModel(wellMeshes, model);
		wellPass->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
		lowPollyTreesPass = new RenderPassModel(polyTreesMeshes, model);
		lowPollyTreesPass->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
		grassPass = new RenderPassModel(grassMeshes, model);
		grassPass->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
		cartPass = new RenderPassModel(cartMeshes, model);
		cartPass->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
		pigPass = new RenderPassModel(pigMeshes, model);
		pigPass->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);
		smaugPass = new RenderPassModel(smaugMeshes, model);
		smaugPass->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);


	}


	// log (sp);

}

GameOculus::GameOculus(bool warp)
{

	warpping = warp;
	init();
	renderSzene();
}


GameOculus::~GameOculus()
{

	delete myModel;
	delete  castle;
	delete chest;
	delete windMill;
	delete viking ;
	delete viking2;
	delete skyDom;
	delete terraModel;
	delete house1;
	delete house2;
	delete house3;
	delete lowPollyHouse;
	delete bush;
	delete wellModel;
	delete lowPollyTreesModel;
	delete grassModel;
	delete cartModel;
	delete pigModel;


	delete trees;
	delete castlePass;
	delete chestPass;
	delete windMillPass;
	delete vikingPass;
	delete skyBoxPass;
	delete terraPass;
	delete housePass;
	delete house2Pass;
	delete house3Pass;
	delete lowPollyPass;
	delete  bushPass;
	delete wellPass;
	delete lowPollyTreesPass;
	delete  grassPass;
	delete  cartPass;
	delete  pigPass;
	delete collector;
	delete smaugModel;
	delete smaugPass;

}




void  GameOculus::renderSzene()
{


	//render loop for game

 	auto holefilling = new HierarchicalHoleFilling( getResolution(window), quad);

	auto tonemapping = new RenderPass( new Quad(), new ShaderProgram("/Filters/fullscreen.vert","/Filters/toneMapperLinear.frag"));
          tonemapping->getFrameBufferObject()->setFrameBufferObjectHandle(l_FBOId);



	render(window, [&](float deltaTime, glm::mat4 projection, glm::mat4 view)
	{
	      glfwGetWindowSize(window, &width, &height);
	
                cout << deltaTime << endl;
		

		/*  MATRIX STUFF */



		simulateLanetcy (latencyFrameCount, view);

		//use this matrix for simulating latency
		glm::mat4 viewMat_old = latencyQueue.front();


		if(warpping)
		{
    
           
			collector
		        -> clear (0, 0, 0,0)
			->  update("uniformView", viewMat_old, "model")
			->  update("uniformView", viewMat_old, "suzane")
			->  update("uniformView", viewMat_old, "demo")	
   		 	->  update("uniformProjection", projection, "model")	
			->  update("uniformProjection", projection, "suzane")
			->  update("uniformProjection", projection, "demo")
			->  run();

			diffWarp
			-> clear (0, 0, 0,0)
			-> update("switchWarp", (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)?1:0)
			-> update("viewMat", view)
			// -> update("invViewProjection", glm::inverse(projection * viewMat_old))
			-> update("projection", projection)
			-> texture("colorTexture", collector->get("fragColor"))
			-> texture("positionTexture", collector->get("fragPosition"))
			-> run();

			holefilling
			->texture(diffWarp->get("fragColor"))
			->run();

			tonemapping
			-> clear (0, 0, 0,0)
			->texture("tex", holefilling->getOutput())
			->update("resolution", getResolution(window))
			->run();

		}
		else
		{



			skyBoxPass
			->  update("uniformModel", skyModel)
			->  update("uniformView", viewMat_old)

			->  run();


			castlePass
			->  update("uniformModel", modelCastle)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update ("viewPosition", camera->getPosition())
			->  update("attenuatFactor", false)
			->  run();

			terraPass
			->  update("uniformModel", modelTerra)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  run();
		

			 housePass
			->  update("uniformModel", modelHouse1)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();


			housePass
			->  update("uniformModel", modelHouse9)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();

			house2Pass
			->  update("uniformModel", modelHouse2)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();



			house3Pass
			->  update("uniformModel", modelHouse3)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();



			house3Pass
			->  update("uniformModel", modelHouse4)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();



			house3Pass
			->  update("uniformModel", modelHouse5)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();



			housePass
			->  update("uniformModel", modelHouse6)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();

			house2Pass
			->  update("uniformModel", modelHouse7)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();

			house3Pass
			->  update("uniformModel", modelHouse8)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();



			lowPollyPass
			->  update("uniformModel", lowHouse1)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();

			lowPollyPass
			->  update("uniformModel", lowHouse2)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();



			lowPollyPass
			->  update("uniformModel", lowHouse3)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();



			lowPollyPass
			->  update("uniformModel", lowHouse4)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();


			lowPollyPass
			->  update("uniformModel", lowHouse5)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();



			lowPollyPass
			->  update("uniformModel", lowHouse6)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)

			->  update("attenuatFactor", false)
			->  run();

			lowPollyPass
			->  update("uniformModel", lowHouse7)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();


			lowPollyPass
			->  update("uniformModel", lowHouse8)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();



			lowPollyPass
			->  update("uniformModel", lowHouse9)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();


			windMillPass
			->  update("uniformModel", modelWindMill)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();

			vikingPass
			->  update("uniformModel", vikingModel2)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  run();

			vikingPass
			->  update("uniformModel", vikingModel)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  run();


			bushPass
			->  update("uniformModel", modelBush1)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();



			bushPass
			->  update("uniformModel", modelBush2)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();


			bushPass
			->  update("uniformModel", modelBush3)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();

			bushPass
			->  update("uniformModel", modelBush4)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();

			bushPass
			->  update("uniformModel", modelBush5)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();

			bushPass
			->  update("uniformModel", modelBush6)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();


			bushPass
			->  update("uniformModel", modelBush7)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();

			wellPass
			->  update("uniformModel", modelWell)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();


			trees
			->  update("uniformModel", modelTree)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();

			trees
			->  update("uniformModel", modelTree1)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();


			trees
			->  update("uniformModel", modelTree2)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();


			trees
			->  update("uniformModel", modelTree3)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();

			trees
			->  update("uniformModel", modelTree4)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();

			trees
			->  update("uniformModel", modelTree5)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();

			trees
			->  update("uniformModel", modelTree6)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();


			trees
			->  update("uniformModel", modelTree7)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();


			trees
			->  update("uniformModel", modelTree8)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();


			trees
			->  update("uniformModel", modelTree9)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();



			lowPollyTreesPass
			->  update("uniformModel", modelPolyTrees1)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();

			lowPollyTreesPass
			->  update("uniformModel", modelPolyTrees2)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();

			lowPollyTreesPass
			->  update("uniformModel", modelPolyTrees3)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();

		       lowPollyTreesPass
			->  update("uniformModel", modelPolyTrees4)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();


			grassPass
			->  update("uniformModel", modelGrass1)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();


			grassPass
			->  update("uniformModel", modelGrass2)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();



			grassPass
			->  update("uniformModel", modelGrass3)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();

			grassPass
			->  update("uniformModel", modelGrass4)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();
			grassPass
			->  update("uniformModel", modelGrass5)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();
			grassPass
			->  update("uniformModel", modelGrass6)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();
			grassPass
			->  update("uniformModel", modelGrass7)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();
			grassPass
			->  update("uniformModel", modelGrass8)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();
			grassPass
			->  update("uniformModel", modelGrass9)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();
			grassPass
			->  update("uniformModel", modelGrass10)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();

			grassPass
			->  update("uniformModel", modelGrass11)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();

			grassPass
			->  update("uniformModel", modelGrass12)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();

			grassPass
			->  update("uniformModel", modelGrass13)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();


			grassPass
			->  update("uniformModel", modelGrass14)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();


			grassPass
			->  update("uniformModel", modelGrass15)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();


			grassPass
			->  update("uniformModel", modelGrass16)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();


			grassPass
			->  update("uniformModel", modelGrass17)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();



			grassPass
			->  update("uniformModel", modelGrass18)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();




			grassPass
			->  update("uniformModel", modelGrass19)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();



			grassPass
			->  update("uniformModel", modelGrass20)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();


			grassPass
			->  update("uniformModel", modelGrass21)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();



			grassPass
			->  update("uniformModel", modelGrass22)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();
			grassPass
			->  update("uniformModel", modelGrass23)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();
			grassPass
			->  update("uniformModel", modelGrass24)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();


			grassPass
			->  update("uniformModel", modelGrass25)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();


			grassPass
			->  update("uniformModel", modelGrass26)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();




			grassPass
			->  update("uniformModel", modelGrass27)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();



			grassPass
			->  update("uniformModel", modelGrass28)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();


			cartPass
			->  update("uniformModel", modelCart)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();

			pigPass
			->  update("uniformModel", modelPig)
			->  update("uniformView", viewMat_old)
			->  update("uniformProjection", projection)
			->  update("lightPos", lightPosition)
			->  update("attenuatFactor", false)
			->  run();



			 chestPass
			        ->  update("uniformModel", modelChest)
			        ->  update("uniformView", viewMat_old)
			        ->  update("uniformProjection", projection)
			        ->  update ("viewPosition", camera->getPosition())
			        ->  update("lightPos", lightPosition)
			        ->  update("shinines", shinines)
			        ->  update("attenuatFactor", false)
			        ->  run();

		        smaugPass
		                -> update("uniformModel", modelSmaug)
			        ->  update("uniformView", viewMat_old)
			        ->  update("uniformProjection", projection)
			        ->  update ("viewPosition", camera->getPosition())
			        ->  update("lightPos", lightPosition)
			        ->  update("shinines", shinines)
			        ->  update("attenuatFactor", false)
			        ->  run();
			
			

		}

	});
    

}



int main(int argc, char *argv[])
{

	GameOculus * g = new GameOculus(argc > 1 );

	delete g;
}
