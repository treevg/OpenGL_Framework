#include "ShaderTools/DefaultRenderLoop.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"
#include "Compression/TextureTools.h"
#include "Compression/ComputeShaderTools.h"
#include "Compression/ColorField.h"

using namespace std;
using namespace glm;

auto sp = new ShaderProgram({"/Compression/test1.vert", "/Compression/test1.frag"});
auto pass = new RenderPass(new Cube(), sp, width, height);
//auto pass = new RenderPass(new Cube(), sp);

auto compositingSP = new ShaderProgram({"/Compression/pass.vert", "/Compression/compositing.frag"});

auto pass2 = new RenderPass(new Quad(), compositingSP);

auto cs = new ShaderProgram(GL_COMPUTE_SHADER, "/Compression/invert.comp");

auto RGBtoYCbCr = new ShaderProgram(GL_COMPUTE_SHADER, "/Compression/rgbToYCbCr.comp");
auto YCbCrToRGB = new ShaderProgram(GL_COMPUTE_SHADER, "/Compression/YCbCrToRGB.comp");
auto compressCbCr = new ShaderProgram(GL_COMPUTE_SHADER, "/Compression/compressCbCr.comp");
auto compressedYCbCrToRGB = new ShaderProgram(GL_COMPUTE_SHADER, "/Compression/compressedYCbCrToRGB.comp");

float cubeAngle = 0.0f;
float rotationSpeed = 0.01f;

glm::mat4 viewMat       = glm::lookAt(
    glm::vec3(0,10,10), // Camera is at (0,10,10), in World Space
    glm::vec3(0,0,0), // and looks at the origin
    glm::vec3(0,1,0));  // Head is up (set to 0, 1 ,0 to look upside-down)

using namespace glm;

glm::mat4 projMat = glm::perspective(45.0f, float(width)/float(height), 0.1f, 100.0f);
mat4 cubeModel = translate(mat4(1.0f), vec3(0.0f, 1.0f, 0.0f));

GLuint textureHandle = TextureTools::loadTexture(RESOURCES_PATH "/cubeTexture.jpg");

GLuint tex1Handle;
GLuint tex2Handle;
GLuint tex3Handle;
GLuint tex4Handle;
GLuint frameBufferObjectHandle;

int tWidth, tHeight;
float *data;


vector<ColorField> doRLE(float *array){
	vector<ColorField> data2;
	int startAddressOfPixel = 0;
	float rOld, gOld, bOld, aOld;
	int count = 1;
	for(int x = 0; x < tWidth; x++){
		for(int y = 0; y < tHeight; y++){
			startAddressOfPixel = ((y * tWidth) + x) * 4;
			float r = array[startAddressOfPixel];
			float g = array[startAddressOfPixel + 4];
			float b = array[startAddressOfPixel + 8];
			float a = array[startAddressOfPixel + 12];

			if (rOld == r && gOld == g && bOld == b && aOld == a){
				count++;
			}
			else{
			rOld = r;
			gOld = g;
			bOld = b;
			aOld = a;

			ColorField *temp = new ColorField(count, r, g, b,a);

			data2.push_back(*temp);
			}
		}
	}

	return data2;
}

int main(int argc, char *argv[]) {
    glGenFramebuffers(1, &frameBufferObjectHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObjectHandle);

    glGenTextures(1, &tex1Handle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex1Handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    // Allocate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex1Handle, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    glGenTextures(1, &tex2Handle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex2Handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    // Allocate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, tex2Handle, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT1);

    glGenTextures(1, &tex3Handle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex3Handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, width/2, height/2, 0, GL_RG, GL_FLOAT, NULL);
    // Allocate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, tex3Handle, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT1);

    glGenTextures(1, &tex4Handle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex4Handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, width/2, height/2, 0, GL_RG, GL_FLOAT, NULL);
    // Allocate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, tex4Handle, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT1);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObjectHandle);
    GLfloat clearColor[4] = {0, 1, 0, 0};
    glClearBufferfv(GL_COLOR, 0, clearColor);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    YCbCrToRGB->printInputInfo();
    YCbCrToRGB->printUniformInfo();
    YCbCrToRGB->printOutputInfo();

    glBindTexture(GL_TEXTURE_2D, tex1Handle);																	//prepare swapping Texture between Memories
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &tWidth);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &tHeight);
    std::cout<<"width: "<< tWidth <<", height: " << height << std::endl;

    data = (float*)malloc( sizeof(float) * tHeight * tWidth * 4);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, data);
    int y=0;
        for( unsigned int i = 0; i < tWidth * tHeight * 4 ; i++ )
                {
                        y++;
                }
    cout<<"size : "<< (float)(sizeof(float) * tHeight * tWidth * 4)/1000000<< " MByte"<<endl;
    cout<<"array: " << y * 4<<endl;
    glBindTexture(GL_TEXTURE_2D, 0);																			//end preparation

    renderLoop([]{
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {glfwDestroyWindow(window); exit(-1);};						//close the window

        //rotate and translate the cube for a neat little animation
        cubeAngle = fmod((cubeAngle + rotationSpeed * glfwGetTime()), (pi<float>() * 2.0f));
        glfwSetTime(0.0);
        cubeModel = translate(rotate(mat4(1.0f), degrees(cubeAngle), vec3(1.0f, 1.0f, 0.0f)), vec3(0.0f, 2.0f, -2.0f));

        pass
        -> clear(1, 1, 1, 0)
        -> update("uniformView", viewMat)
        -> update("uniformProjection", projMat)
        -> update("uniformModel", cubeModel)
        -> texture("tex2", textureHandle)
        -> run();

//        cs->use();

        RGBtoYCbCr->use();
        glBindImageTexture(0, pass->get("fragColor"), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
        glBindImageTexture(1, tex1Handle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
        glDispatchCompute(int(width/16), int(height/16), 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        compressCbCr->use();
        glBindImageTexture(0, tex1Handle, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
        glBindImageTexture(1, tex3Handle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RG32F);
        glBindImageTexture(2, tex4Handle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RG32F);
        glDispatchCompute(int(width/16), int(height/16), 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        YCbCrToRGB->use();
        glBindImageTexture(0, tex1Handle, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
        glBindImageTexture(1, tex2Handle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
        glDispatchCompute(int(width/16), int(height/16), 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        glBindTexture(GL_TEXTURE_2D, tex1Handle);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, data);
        glBindTexture(GL_TEXTURE_2D, 0);

        vector<ColorField> test = doRLE(data);

        cout<<test.size() * sizeof(float) *4<<endl;

        glBindTexture(GL_TEXTURE_2D, tex1Handle);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tWidth, tHeight, GL_RGBA, GL_FLOAT, data);
        glBindTexture(GL_TEXTURE, 0);





//        compressedYCbCrToRGB->use();
//        glBindImageTexture(0, tex3Handle, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RG16F);
//        glBindImageTexture(1, tex4Handle, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RG32F);
//        glBindImageTexture(2, tex2Handle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
//        glDispatchCompute(int(width/16), int(height/16), 1);
//        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        pass2
        ->clear(1, 1, 1, 0)
//        ->texture("tex2", pass->get("fragColor"))
        ->texture("tex2", tex1Handle)
        ->run();

//        glBindTexture(GL_TEXTURE_2D, tex1Handle);
//        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, data);
//        glBindTexture(GL_TEXTURE_2D, 0);

    });
}
