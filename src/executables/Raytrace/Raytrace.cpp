#include "ShaderTools/DefaultRenderLoop.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include <array>
#include <queue>

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
//TODO interpolate normals for correct shading and pass them as texture

//		Low priority

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
int		warpOnOff=0;
float 	warpUpDown = 0.0;
float	warpLeftRight = 0.0;

vector<vec4> sphereVec;
vector<vec3> mesh;
vector<vec3> colorSphere;
vector<vec3> colorTriangle;
vector<mat4> matVec;

// latency stuff
int latencyFrameNumber = 12;
queue<mat4> latencyQueue;


auto quadVAO = new Quad();
auto grid = new Grid(width,height);

//Load mesh: parameter is resources path
auto ssbo2 = new ShaderStorageBuffer("/Objects/icosphere2.obj", false);
//auto ssbo = new ShaderStorageBuffer();

// basics of fragment shader taken from: https://www.shadertoy.com/view/ldS3DW
// triangle intersection taken from: http://undernones.blogspot.de/2010/12/gpu-ray-tracing-with-glsl.html
auto sp = new ShaderProgram({"/Raytracing/raytrace.vert", "/Raytracing/raytrace2.frag"});
auto raytracePass = new RenderPass(quadVAO, sp
    ,width, height
    );

//Tonemap shader for depth
auto spLin = new ShaderProgram({"/Raytracing/raytrace.vert", "/Raytracing/toneMap.frag"});
auto tonemappingPass = new RenderPass(quadVAO,spLin
    // ,width, height
    );

//For Compression
auto sp2 = new ShaderProgram({"/Compression/test1.vert", "/Compression/test1.frag"});
auto pass2 = new RenderPass(new Cube(), sp2);
// GLuint textureHandle = TextureTools::loadTexture(RESOURCES_PATH "/bambus.jpg");
GLuint texHandle = ComputeShaderTools::generateTexture();

//Composite shader
auto compSP = new ShaderProgram({"/Raytracing/raytrace.vert", "/Raytracing/compositing.frag"});
auto compositing = new RenderPass(quadVAO, compSP);

//Warping shader
auto warp = new ShaderProgram({"/Raytracing/warp.vert", "/Raytracing/warp.frag"});
auto diffWarp = new RenderPass(grid, warp);

// Simulate latency
// mat4 latency(mat4 newMat, int lat){
//     latencyQueue.push_back(newMat);



//     matVec.push_back(newMat);
//     int lengthMatVec = matVec.size();

//     mat4 returnMat;
//     if(lengthMatVec<=lat){
//         return newMat;
//     }
//     else {
//         matVec.erase(matVec.begin());
//         return returnMat = matVec[lengthMatVec-lat-1];
//     }
// }


int main(int argc, char *argv[]) {

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

       sp -> printUniformInfo();
      // sp -> printInputInfo();
      // sp -> printOutputInfo();

    // sphereVec.push_back(glm::vec4(2.0, 0.0, 0.0, 0.2));
    // sphereVec.push_back(glm::vec4(2.75, 0.5, 0.5, 0.2));
    // sphereVec.push_back(glm::vec4(1.75, 0.5, 0.5, 0.2));

    sphereVec.push_back(glm::vec4(0.0, 0.0, 0.0, 0.5));
    sphereVec.push_back(glm::vec4(0.75, 0.5, 0.5, 0.5));
    sphereVec.push_back(glm::vec4(-0.75, 0.5, 0.5, 0.5));

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

   // raytracePass -> update("mesh[0]", mesh);
    raytracePass -> update("sphereVec[0]", sphereVec);
    raytracePass -> update("colorSphere[0]", colorSphere);
    raytracePass -> update("colorTriangle[0]", colorTriangle);


    renderLoop([]{
        currentTime = glfwGetTime();
        float deltaT = currentTime - lastTime;
        lastTime = currentTime;
        bool mouseForGerrit = false;

        if(!mouseForGerrit){
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
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            cout << (rad) << endl;
            rad +=0.03 * deltaT;
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            cout << (rad) << endl;
            rad -= 0.03 * deltaT;
        }


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

        // Warpview
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) warpUpDown +=  0.1 * deltaT;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) warpUpDown -=  0.1 * deltaT;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) warpLeftRight -=  0.1* deltaT ;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) warpLeftRight += 0.1* deltaT ;


        if(minRange>=maxRange){
        	minRange=1.0;
        	maxRange=20.0;
        }

        mat4 projection = perspective(45.0f, float(width)/float(height), 0.1f, 100.0f);
        mat4 invProjection = inverse(projection);

        //original
        mat4 view(1);
        view = translate(view, vec3(0,0,-4));
        view = rotate(view, -verticalAngle, vec3(1,0,0));
        view = rotate(view, -horizontalAngle, vec3(0,1,0));

        // Latency simulation
        latencyQueue.push(view);
        while (latencyQueue.size() > latencyFrameNumber) {
            latencyQueue.pop();
        }
        mat4 view_old = latencyQueue.front();

        mat4 invView = inverse(view);
        mat4 invView_old = inverse(view_old);

        mat4 invViewProjection = inverse(projection * view);
        mat4 invViewProjection_old = inverse(projection * view_old);


       //  //slightly different VM
       //  mat4 aView (1);
       //  aView = translate(aView, vec3(0.05, 0, -4.0));
       //  aView = rotate(aView, warpUpDown-verticalAngle, vec3(1,0,0));
       //  aView = rotate(aView, warpLeftRight-horizontalAngle, vec3(0,1,0));

       // // mat4 latAView = latency(aView, 20);
       //  mat3 modIncTrans = transpose(inverse(mat3(1)));

       //  vec4 pos = invView * vec4(0,0,0,1);
       //  vec4 dir = normalize(invView * vec4(0,0,1,0));


            ssbo2->bind(11);

        	raytracePass
        	-> clear(0, 0, 0, 0)
        	-> update("iResolution", glm::vec3(width, height, 1))
        	-> update("zoom", rad)
            -> update("invViewProjection", invViewProjection_old)
        	-> update("invView",invView_old)
            -> update("enter", (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)?1:0)
        	-> run();


   //          compositing
            // -> clear(0, 1, 0, 0)
            // -> update("texNum", texNum)
            // -> texture("color", raytracePass->get("fragColor"))
            // -> texture("indirectionColor", raytracePass->get("fragColor2"))
            // -> texture("depth", raytracePass->get("fragDepth"))
            // -> texture("indirectionDepth", raytracePass->get("fragDepth2"))
            // //-> texture("fragPos", raytracePass->get("fragPosition"))
            // //-> texture("indirectionFragPos", raytracePass->get("fragPosition2"))
            // -> run();
            
            if (glfwGetKey(window, GLFW_KEY_Y /*Z*/) == GLFW_PRESS) {
                tonemappingPass
                    -> clear(0,0,0,0)
                    -> update("minRange",minRange)
                    -> update("maxRange",maxRange)
                    -> texture("depth", raytracePass->get("fragDepth"))
                    -> run();
            } else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
            	tonemappingPass
        			-> clear(0,0,0,0)
        			-> update("minRange",0.0f)
        			-> update("maxRange",1.0f)
        			-> texture("depth", raytracePass->get("fragPosition"))
    			    -> run();
            } else  {            
                diffWarp
        			-> clear(0,0,0,0)
                    -> update("warpOnOff", (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)?1:0)
        			-> update("altView", view)
        			-> update("invViewProjection", invViewProjection_old)
        			-> update("projection", projection)
        			-> texture("colorTexture", raytracePass->get("fragColor"))
                    -> texture("depthTexture", raytracePass->get("fragDepth"))
        			-> texture("positionTexture", raytracePass->get("fragPosition"))
        			-> texture("indirectColor", raytracePass->get("fragColor2"))
        			-> run();

        }

    });
}
