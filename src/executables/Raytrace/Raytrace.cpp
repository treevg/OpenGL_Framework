#include "ShaderTools/DefaultRenderLoop.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include <array>

#include "ShaderTools/VertexArrayObjects/Cube.h"
#include "Compression/TextureTools.h"
#include "Compression/ComputeShaderTools.h"
#include "ShaderTools/VertexArrayObjects/Grid.h"
//#include "Objectloader/Objectloader.h"

#include "ShaderTools/ShaderStorageBuffer.h"

using namespace std;
using namespace glm;

//		High priority
//TODO try reflective warping
//TODO normals for correct shading - mesh structure?

//		Low priority
//TODO switch between textures
//TODO Himmel soll nachziehen bei Rotation - Skybox?

//global variables

//float 	size = 1.0;
//float 	lum =  0.5;
float 	rad=0.0;
double 	xpos, ypos;
int 	texNum=0;
float 	minRange=0.8, maxRange=10;
float 	lastTime, currentTime;
float 	horizontalAngle=0.0;
float 	verticalAngle=0.0;
float 	offsetHorizontalAngle=0.01;
float 	offsetVerticalAngle=-0.01;
int		warpView=0;
float 	warpUpDown = 0.0;
float	warpLeftRight = 0.0;

vector<vec4> sphereVec;
vector<vec3> mesh;
vector<vec3> colorSphere;
vector<vec3> colorTriangle;
vector<mat4> matVec;

auto quadVAO = new Quad();
auto grid = new Grid(width,height);

//Load mesh: parameter is resources path
auto ssbo2 = new ShaderStorageBuffer("/Objects/originalMesh.obj");
//auto ssbo = new ShaderStorageBuffer();

// basics of fragment shader taken from: https://www.shadertoy.com/view/ldS3DW
// triangle intersection taken from: http://undernones.blogspot.de/2010/12/gpu-ray-tracing-with-glsl.html
auto sp = new ShaderProgram({"/Raytracing/raytrace.vert", "/Raytracing/raytrace2.frag"});
auto pass1 = new RenderPass(quadVAO, sp,width, height);

//Tonemap shader for depth
auto spLin = new ShaderProgram({"/Raytracing/raytrace.vert", "/Raytracing/toneMap.frag"});
auto passLin = new RenderPass(quadVAO,spLin,width, height);

//For Compression
auto sp2 = new ShaderProgram({"/Compression/test1.vert", "/Compression/test1.frag"});
auto pass2 = new RenderPass(new Cube(), sp2);
GLuint textureHandle = TextureTools::loadTexture(RESOURCES_PATH "/bambus.jpg");
GLuint texHandle = ComputeShaderTools::generateTexture();

//Composite shader
auto compSP = new ShaderProgram({"/Raytracing/raytrace.vert", "/Raytracing/compositing.frag"});
auto compositing = new RenderPass(quadVAO, compSP);

//Warping shader
auto warp = new ShaderProgram({"/Raytracing/warp.vert", "/Raytracing/warp.frag"});
auto diffWarp = new RenderPass(grid, warp);

// Simulate latency
mat4 latency(mat4 newMat, int lat){
matVec.push_back(newMat);
int lengthMatVec = matVec.size();

mat4 returnMat;
if(lengthMatVec<=lat){
	return newMat;
}
else {
	matVec.erase(matVec.begin());
	return returnMat = matVec[lengthMatVec-lat-1];
	}
}

int main(int argc, char *argv[]) {

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

//       warp -> printUniformInfo();
//       warp -> printInputInfo();
//       warp -> printOutputInfo();

    sphereVec.push_back(glm::vec4(2.0, 0.0, 0.0, 0.2));
    sphereVec.push_back(glm::vec4(2.75, 0.5, 0.5, 0.2));
    sphereVec.push_back(glm::vec4(1.75, 0.5, 0.5, 0.2));

    //needs to be same size as sphereVec
    colorSphere.push_back(glm::vec3(0.8,0.4,0.4));
    colorSphere.push_back(glm::vec3(0.4,0.8,0.4));
    colorSphere.push_back(glm::vec3(0.4,0.4,0.8));

    colorTriangle.push_back(glm::vec3(0.4,0.7,0.7));
    colorTriangle.push_back(glm::vec3(0.7,0.7,0.4));
    colorTriangle.push_back(glm::vec3(0.6,0.4,0.8));
    colorTriangle.push_back(glm::vec3(0.7,0.3,0.3));
    colorTriangle.push_back(glm::vec3(0.1,0.4,0.8));

    // initialize mesh
    mesh.push_back(glm::vec3(0.5, -0.5, 1.5));
    mesh.push_back(glm::vec3(0.0, 0.8, 1.5));
    mesh.push_back(glm::vec3(-0.5, -0.5, 1.5));
    mesh.push_back(glm::vec3(0.0, 0.5, 1.0));
    mesh.push_back(glm::vec3(-0.5, 1.8, 1.0));
    mesh.push_back(glm::vec3(-1., 0.5, 1.0));

    lastTime = glfwGetTime();

   // pass1 -> update("mesh[0]", mesh);
    pass1 -> update("sphereVec[0]", sphereVec);
    pass1 -> update("colorSphere[0]", colorSphere);
    pass1 -> update("colorTriangle[0]", colorTriangle);


    renderLoop([]{
        currentTime = glfwGetTime();
        float deltaT = currentTime - lastTime;
        lastTime = currentTime;
        bool mouseForGerrit = true;

        if(!mouseForGerrit){
//             double xold = xpos;
//             double yold = ypos;
//             xpos = xpos- xold;
//             ypos = ypos-yold;
             glfwGetCursorPos(window, &xpos,&ypos);
             glfwSetCursorPos(window, float(width)/2, float(height)/2);

             horizontalAngle += 0.2 * deltaT * float(width/2 - xpos );
             verticalAngle   += 0.2 * deltaT * float( height/2 - ypos );

        }
        else{
        if (glfwGetMouseButton(window, 0) == GLFW_PRESS) {
            glfwGetCursorPos(window, &xpos,&ypos);
            xpos = xpos / (double)width * 2.0 - 1.0;
            ypos = ypos / (double)height * 2.0 - 1.0;
            horizontalAngle = xpos;
            verticalAngle   = ypos;
        }

        }

       // cout << xpos << " " << ypos << endl;

//        warpUpDown = horizontalAngle;
//        warpLeftRight = verticalAngle;

//
//        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) size  = glm::max(size - 0.5 * deltaT, 0.);
//        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) size = glm::min(size + 0.5 * deltaT, 1.);
//        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) lum  = glm::max(lum - 0.5 * deltaT, 0.);
//        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) lum = glm::min(lum + 0.5 * deltaT, 1.);

        // Close window
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);

        // Zoom
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) rad +=0.03 * deltaT;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)rad -= 0.03 * deltaT;


        // Change between rendered textures
        if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)texNum =0;
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)texNum =0;
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)texNum =1;
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)texNum =2;
        if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)texNum =3;
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)texNum =4;

        // Linearize Depth
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)minRange +=0.1;
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)minRange -=0.1;
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)maxRange +=0.5;
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)maxRange -=0.5;

        // Warpview on/off
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)warpView =1;
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)warpView =0;

        // Warpview
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) warpUpDown +=  0.1 * deltaT;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) warpUpDown -=  0.1 * deltaT;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) warpLeftRight -=  0.1* deltaT ;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) warpLeftRight += 0.1* deltaT ;


        if(minRange>=maxRange){
        	minRange=1.0;
        	maxRange=20.0;
        }


        //original
        mat4 view(1);
        view = translate(view, vec3(0,0,-4));
        view = rotate(view, -verticalAngle, vec3(1,0,0));
        view = rotate(view, -horizontalAngle, vec3(0,1,0));

        mat4 invView = inverse(view);
        mat4 lat = latency(invView,10);


        //slightly different VM
        mat4 aView (1);
        aView = translate(aView, vec3(0.05, 0, -4.0));
        aView = rotate(aView, warpUpDown-verticalAngle, vec3(1,0,0));
        aView = rotate(aView, warpLeftRight-horizontalAngle, vec3(0,1,0));

       // mat4 latAView = latency(aView, 20);
        mat3 modIncTrans = transpose(inverse(mat3(1)));

        vec4 pos = invView * vec4(0,0,0,1);
        vec4 dir = normalize(invView * vec4(0,0,1,0));

        mat4 projection = perspective(45.0f, float(width)/float(height), 0.1f, 100.0f);
        mat4 invProjection = inverse(projection);
        mat4 invViewProjection = inverse(projection * view);
       // cout << to_string(dir) << endl;

        if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS) {
            pass2
            -> clear(1, 1, 1, 0)
            -> update("uniformView", view)
            -> update("uniformProjection", glm::perspective(45.0f, float(width)/float(height), 0.1f, 100.0f))
            -> update("uniformModel", mat4(1))
            -> texture("tex2", textureHandle)
            -> run();
        }
        else {

            ssbo2->bind(7);
        	pass1
        	-> clear(0, 0, 0, 0)
        	-> update("iResolution", glm::vec3(width, height, 1))
        	-> update("zoom", rad)
            -> update("invViewProjection", invViewProjection)
        	-> update("invView",lat)
			-> update("normalMat",modIncTrans)
            -> update("enter", (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)?1:0)
        	-> run();

        	passLin
			-> clear(0,0,0,0)
			-> update("minRange",minRange)
			-> update("maxRange",maxRange)
			-> texture("depth", pass1->get("fragDepth"))
			-> run();

            compositing
			-> clear(0, 1, 0, 0)
			-> update("texNum", texNum)
			-> texture("color", pass1->get("fragColor"))
			-> texture("indirectionColor", pass1->get("fragColor2"))
			-> texture("depth", passLin->get("fragColor"))
			-> texture("indirectionDepth", pass1->get("fragDepth2"))
			//-> texture("fragPos", pass1->get("fragPosition"))
			//-> texture("indirectionFragPos", pass1->get("fragPosition2"))
			-> run();

           diffWarp
			-> clear(0,0,0,0)
            -> update("warpView", warpView)
			-> update("altView", aView)
			-> update("invViewProjection", invViewProjection)
			-> update("projection", projection)
			-> texture("color", pass1->get("fragColor"))
			-> texture("depth", passLin->get("fragColor"))
			-> texture("indirectColor", pass1->get("fragColor2"))
			-> run();

        }

    });
}
