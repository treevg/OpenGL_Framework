#include "ShaderTools/DefaultRenderLoop.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include <array>


// fragment shader taken from: https://www.shadertoy.com/view/ldS3DW
// original shader was "../Moritz_Ba/raytrace.frag"
auto sp = new ShaderProgram({"/Test_ShaderTools/Moritz_Ba/raytrace.vert", "/Test_ShaderTools/Moritz_Ba/raytrace3.frag"});

auto pass = new RenderPass(
    new Quad(), 
    sp
);

//TODO load own spheres / multiple spheres
//TODO avoid horizonatal line / change background?
//TODO make lightsource a fix point?
//TODO getTextures/depth/color  -> multiple passes
//TODO occlusion by other spheres
//TODO spheres with colors?



float size = 0.5;
float lum = 0.5;
int arraySize;
glm::vec3 eye;


glm::vec4 sphere1 = glm::vec4(0.0, 0.25, 0.0, 0.5);  // vec4(.x, .y, .z, rad)
glm::vec4 sphere2 = glm::vec4(0.5, 0.5, -0.5, 0.5);

float spheres[4]={
		0.0, 0.25, 0.0, 0.5
};

float lastTime, currentTime;

/*
glm::mat4 viewMat = {
		1,	0,	0,	0,
		0,	1,	0,	0,
		0,	0,	1,	0,
		0,	0,	0,	1,
};



glm::mat4 viewMat       = glm::lookAt(
    glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
    glm::vec3(0,0,0), // and looks at the origin
    glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
);


glm::mat4 projMat = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

glm::mat4 mvp= projMat * viewMat;
*/

int main(int argc, char *argv[]) {
    sp -> printUniformInfo();
    sp -> printInputInfo();
    sp -> printOutputInfo();

    // fill sphere-array
    //spheres[0]= sphere1;
   // spheres[1]= sphere2;


   arraySize= sizeof(spheres)/sizeof(*spheres);
   eye=glm::vec3(0.0,0.0,-3.0);


    lastTime = glfwGetTime();

    renderLoop([]{
        currentTime = glfwGetTime();
        float deltaT = currentTime - lastTime;
        lastTime = currentTime;


        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) size  = glm::max(size - 0.5 * deltaT, 0.);
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) size = glm::min(size + 0.5 * deltaT, 1.);
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) lum  = glm::max(lum - 0.5 * deltaT, 0.);
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) lum = glm::min(lum + 0.5 * deltaT, 1.);

        pass
        -> clear(0, 0, 0, 0)

		-> update("iGlobalTime", lastTime)
		-> update("iResolution", glm::vec3(1280, 720, 1))

		-> update("spheres", spheres) //doesnt work properly
		//-> update("arraySize", arraySize)

		-> update("eye", eye)
        -> update("color", glm::vec4(1,0,0,1))
        -> update("sphere1", sphere1)
		-> update("sphere2", sphere2)
		-> update("nicht_gesetzte_uniform", glm::vec4(1, 3, 3, 7))
        -> update("scale", size)
        -> update("luminance", lum)
        -> run();
    });
}
