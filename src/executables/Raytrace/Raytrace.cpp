#include "ShaderTools/DefaultRenderLoop.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include <array>

#include "ShaderTools/VertexArrayObjects/Cube.h"
#include "Compression/TextureTools.h"
#include "Compression/ComputeShaderTools.h"
#include "ShaderTools/VertexArrayObjects/Grid.h"
#include "Objectloader/Objectloader.h"

using namespace std;
using namespace glm;

//TODO make objectloader class

auto quadVAO = new Quad();
auto grid = new Grid(width,height);

auto objl = new Objectloader();

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


//global variables
float 	size = 1.0;
float 	lum =  0.5;
float 	rad=0.0;
double 	xpos, ypos;
int 	ref=1;
int 	texNum=0;
float 	minRange=0.8, maxRange=10;
float 	lastTime, currentTime;
float 	horizontalAngle=0.0;
float 	verticalAngle=0.0;
int		warpView=0;

std::vector<glm::vec4> sphereVec;
std::vector<glm::vec3> mesh;
std::vector<glm::vec3> colorSphere;
std::vector<glm::vec3> colorTriangle;


int main(int argc, char *argv[]) {

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

//    sp -> printUniformInfo();
//    sp -> printInputInfo();
//    sp -> printOutputInfo();
       warp -> printUniformInfo();
       warp -> printInputInfo();
       warp -> printOutputInfo();

//    compSP -> printUniformInfo();
//    compSP -> printInputInfo();
//    compSP -> printOutputInfo();

    // Load mesh
    objl->loadOBJ(RESOURCES_PATH "/Objects/originalMesh.obj", objl->vertices, objl->uvs, objl->normals);



    sphereVec.push_back(glm::vec4(0.0, 0.0, 0.0, 0.5));
    sphereVec.push_back(glm::vec4(0.75, 0.5, 0.5, 0.5));
    sphereVec.push_back(glm::vec4(-0.75, 0.5, 0.5, 0.5));

    //needs to be same size as sphereVec
    colorSphere.push_back(glm::vec3(0.8,0.4,0.4));
    colorSphere.push_back(glm::vec3(0.4,0.8,0.4));
    colorSphere.push_back(glm::vec3(0.4,0.4,0.8));

    colorTriangle.push_back(glm::vec3(0.4,0.7,0.7));
    colorTriangle.push_back(glm::vec3(0.7,0.7,0.4));
    colorTriangle.push_back(glm::vec3(0.4,0.4,0.8));

    // initialize mesh
    mesh.push_back(glm::vec3(0.5, -0.5, 1.5));
    mesh.push_back(glm::vec3(0.0, 0.8, 1.5));
    mesh.push_back(glm::vec3(-0.5, -0.5, 1.5));

    mesh.push_back(glm::vec3(0.5, 0.5, 0.75));
    mesh.push_back(glm::vec3(0.0, 1.8, 1.0));
    mesh.push_back(glm::vec3(-0.5, 0.5, 1.5));

//    mesh.push_back(glm::vec3(0.5, -0.5, 1.0));
//    mesh.push_back(glm::vec3(0.8, 0.25, 1.25));
//    mesh.push_back(glm::vec3(0.0, 0.8, 1.0));

//    mesh.push_back(glm::vec3(0.5, 1.5, 1.0));
//    mesh.push_back(glm::vec3(0.8, -1.25, 1.25));
//    mesh.push_back(glm::vec3(0.0, -1.8, 1.0));
//
//    mesh.push_back(glm::vec3(0.0, 0.0, 0.0));
//    mesh.push_back(glm::vec3(0.0, -2.25, 2.25));
//    mesh.push_back(glm::vec3(2.0, 2.8, 2.0));


    lastTime = glfwGetTime();

    pass1 -> update("sphereVec[0]", sphereVec);
    pass1 -> update("mesh[0]", mesh);
    pass1 -> update("colorSphere[0]", colorSphere);
    pass1 -> update("colorTriangle[0]", colorTriangle);
   // pass1 -> update("mesh[0]", objl->vertices);

    renderLoop([]{
        currentTime = glfwGetTime();
        float deltaT = currentTime - lastTime;
        lastTime = currentTime;

        glfwGetCursorPos(window, &xpos,&ypos);
        glfwSetCursorPos(window, float(width)/2, float(height)/2);
        horizontalAngle += 0.5 * deltaT * float(float(width)/2 - xpos );
        verticalAngle   += 0.5 * deltaT * float( float(height)/2 - ypos );


        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) size  = glm::max(size - 0.5 * deltaT, 0.);
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) size = glm::min(size + 0.5 * deltaT, 1.);
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) lum  = glm::max(lum - 0.5 * deltaT, 0.);
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) lum = glm::min(lum + 0.5 * deltaT, 1.);
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) rad +=0.03 * deltaT;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)rad -= 0.03 * deltaT;
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)ref =1;
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)ref =2;
        if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)ref =0;
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)ref =3;

        if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)texNum =0;
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)texNum =0;
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)texNum =1;
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)texNum =2;
        if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)texNum =3;
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)texNum =4;

        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)minRange +=0.1;
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)minRange -=0.1;
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)maxRange +=0.5;
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)maxRange -=0.5;

        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)warpView =1;
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)warpView =0;

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

        //slightly different VM
        mat4 altView(1);
        altView = translate(altView, vec3(0.1,0,-4.0));
        altView = rotate(altView, -verticalAngle, vec3(1,0,0));
        altView = rotate(altView, -horizontalAngle, vec3(0,1,0));

       // mat4 invAltView = inverse(altView);

        vec4 pos = invView * vec4(0,0,0,1);
        vec4 dir = normalize(invView * vec4(0,0,1,0));

        mat4 projection = perspective(45.0f, float(width)/float(height), 0.1f, 100.0f);
        mat4 invProjection = inverse(projection);
        mat4 invViewProjection = inverse(projection * view);
        // cout << to_string(dir) << endl;

        mat4 rotationOnly = mat4( 1.0, 1.0, 1.0, 0.0,
        					  	  1.0, 1.0, 1.0, 0.0,
								  1.0, 1.0, 1.0, 0.0,
								  0.0, 0.0, 0.0, 1.0 );

        mat4 altInvViewProjection = inverse(projection * view * rotationOnly);


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

        	pass1
        	-> clear(0, 0, 0, 0)
        	-> update("iResolution", glm::vec3(width, height, 1))
        	-> update("zoom", rad)
            -> update("invViewProjection", invViewProjection)
        	-> update("invView",invView)
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
			-> update("altView", altView)
			-> update("view", view)
			-> update("rotationOnly",rotationOnly)
			-> update("invViewProjection", invViewProjection)
			-> update("altInvViewProjection", altInvViewProjection)
			-> update("projection", projection)
			-> texture("color", pass1->get("fragColor"))
			-> texture("depth", passLin->get("fragColor"))
			-> run();

        }

    });
}
