#include "ShaderTools/DefaultRenderLoop.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"
#include "Compression/TextureTools.h"
#include "Compression/ComputeShaderTools.h"
#include "Compression/ColorField.h"
#include <sstream>
#include <glm/gtx/string_cast.hpp>

using namespace std;
using namespace glm;

auto sp = new ShaderProgram("/Compression/test1.vert", "/Compression/test1.frag");
auto pass = new RenderPass(new Cube(), sp, width, height);
//auto pass = new RenderPass(new Cube(), sp);

auto compositingSP = new ShaderProgram("/Compression/pass.vert", "/Compression/compositing.frag");

auto pass2 = new RenderPass(new Quad(), compositingSP);

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

double mouseX, mouseY;																	//stubs for mouse coordinates
vector<float> pixelColor(4);																//container for color of texture at mouse coordinates

int tWidth, tHeight;																	//stub for dimensions of texture in CPU-Memory
float *data;																			//container for texture in CPU-Memory as plain array

double oldTime, newTime;

vector<ColorField> doRLE(float *array){
	vector<ColorField> data;
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

			data.push_back(*temp);
			count = 1;
			}
		}
	}

	return data;
}

vector<ColorField> doRLE2(float *array){
	vector<ColorField> data;
	int i = 0;
	float rOld = array[0];
	float gOld = array[4];
	float bOld = array[8];
	float aOld = array[12];
	int count = 0;
	for(int i = 0; i < tWidth * tHeight; i+=16){
		float r = array[i];
		float g = array[i + 4];
		float b = array[i + 8];
		float a = array[i + 12];
		if (rOld == r && gOld == g && bOld == b && aOld == a){
			count++;
			rOld = r;
			gOld = g;
			bOld = b;
			aOld = a;
		}
		else{
		ColorField *temp = new ColorField(count, rOld, gOld, bOld, aOld);
		data.push_back(*temp);
		rOld = r;
		gOld = g;
		bOld = b;
		aOld = a;
		count = 1;
		}
	}
	return data;
}

float* doRLEDecode(vector<ColorField> data){
	float *array;
	int startAddressOfPixel = 0;
	int count = 1;
	for(int x = 0; x < tWidth; x++){
		for(int y = 0; y < tHeight; y++){
			int i = data.front().appearence;
			float r = data.front().r;
			float g = data.front().g;
			float b = data.front().b;
			float a = data.front().a;

			for(int j = 1; j <=i; j++){
				array[startAddressOfPixel] = r;
				array[startAddressOfPixel+4] = g;
				array[startAddressOfPixel+8] = b;
				array[startAddressOfPixel+12] = a;
			}
			if(i != 1)
				y += i;
			}
		}

	return array;
}

double calculateFPS(double interval = 1.0 , std::string title = "NONE"){
	static double tZero = glfwGetTime();
	static double fps = 0.0;

	static double frames = -1.0;

	frames ++;

	if (interval < 0.0)
		interval = 0.0;
	else
		if (interval > 10.0)
			interval = 10;

	double timeElapsed = glfwGetTime() - tZero;

	if (timeElapsed > interval){
		fps = frames / timeElapsed;
		if (title != "NONE"){
			std::ostringstream stream;
			stream << fps;
			std::string fpsToString = stream.str();

			title += " || Frames per second: " + fpsToString;

			const char* pszConstString = title.c_str();
			glfwSetWindowTitle(window, pszConstString);
		}
		else {
			std::cout << "Frames per second: " + std::to_string(fps) << endl;
		}
		frames = 0.0;
		tZero = glfwGetTime();
	}

	return fps;
}

int main(int argc, char *argv[]) {
    glGenFramebuffers(1, &frameBufferObjectHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObjectHandle);

    glGenTextures(1, &tex1Handle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex1Handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex1Handle, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    glGenTextures(1, &tex2Handle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex2Handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, tex2Handle, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT1);

    glGenTextures(1, &tex3Handle);																//this is going to be CbCr Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex3Handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, width/2, height, 0, GL_RG, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, tex3Handle, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT2);

    glGenTextures(1, &tex4Handle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex4Handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, width, height, 0, GL_RG, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, tex4Handle, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT3);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObjectHandle);
    GLfloat clearColor[4] = {0, 1, 0, 0};
    glClearBufferfv(GL_COLOR, 0, clearColor);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindTexture(GL_TEXTURE_2D, tex1Handle);																	//prepare swapping Texture between Memories
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &tWidth);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &tHeight);
    std::cout<<"width: "<< tWidth <<", height: " << height << std::endl;

    data = (float*)malloc( sizeof(float) * tHeight * tWidth * 4);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, data);														//this is where the swapping magic happens
    int y=0;
        for( unsigned int i = 0; i < tWidth * tHeight * 4 ; i++ )
                {
                        y++;
                }
    cout<<"array has: " << y << " entries, which makes a total of ..." << endl;
    cout<<"... size : "<< (float)(sizeof(float) * tHeight * tWidth * 4)/1000000<< " MByte"<<endl;
    glBindTexture(GL_TEXTURE_2D, 0);																			//end preparation --> image is swapped and stored in "data"

    renderLoop([]{
		    calculateFPS(1.0, "OpenGL Window");
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {glfwDestroyWindow(window); exit(-1);};						//close the window
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
     	   glfwGetCursorPos(window, &mouseX, &mouseY);
     	   cout<<"Position of mouse X:" << mouseX << ", Y:" << height - mouseY<<endl;
     	   glReadPixels(mouseX, height - mouseY, 1, 1, GL_RGBA, GL_FLOAT, &pixelColor[0]);
     	   cout<< "Color in that pixel   R: "<< pixelColor[0] << " G: " << pixelColor[1] << " B: " << pixelColor[2] << " A: " << pixelColor[3]<< endl;
        };


        oldTime = newTime;
        newTime = glfwGetTime();
        double delta = newTime - oldTime;
        //rotate and translate the cube for a neat little animation
        cubeAngle = fmod((cubeAngle + rotationSpeed * delta), (pi<float>() * 2.0f));
        //glfwSetTime(0.0);
        cubeModel = translate(rotate(mat4(1.0f), degrees(cubeAngle), vec3(1.0f, 1.0f, 0.0f)), vec3(0.0f, 2.0f, -2.0f));

        pass
        -> clear(1, 1, 1, 0)
        -> update("uniformView", viewMat)
        -> update("uniformProjection", projMat)
        -> update("uniformModel", cubeModel)
        -> texture("tex2", textureHandle)
        -> run();

        RGBtoYCbCr->use();
        glBindImageTexture(0, pass->get("fragColor"), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);		//INPUT texture
        glBindImageTexture(1, tex1Handle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);				//OUTPUT texture
        glDispatchCompute(int(width/16), int(height/16), 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        compressCbCr->use();
        glBindImageTexture(0, tex1Handle, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);					//INPUT texture
        glBindImageTexture(1, tex3Handle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RG32F);					//OUTPUT texture1  Chroma-Channels (Cb, Cr)
        glBindImageTexture(2, tex4Handle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RG32F);					//OUTPUT texture2  Brightness-Channel (Y) and Depth-Channel/transperancy (A)
        glDispatchCompute(int(width/16), int(height/16), 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

//        YCbCrToRGB->use();
//        glBindImageTexture(0, tex1Handle, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
//        glBindImageTexture(1, tex2Handle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
//        glDispatchCompute(int(width/16), int(height/16), 1);
//        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        compressedYCbCrToRGB->use();
        glBindImageTexture(0, tex3Handle, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RG32F);					//INPUT texture1  Chroma-Channels (Cb, Cr)
        glBindImageTexture(1, tex4Handle, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RG32F);					//INPUT texture2  Brightness-Channel (Y) and Depth-Channel (A)
        glBindImageTexture(2, tex2Handle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);				//OUTPUT texture RGBA
        glDispatchCompute(int(width/16), int(height/16), 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        glBindTexture(GL_TEXTURE_2D, tex2Handle);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, data);
        glBindTexture(GL_TEXTURE_2D, 0);

        double lastTime = glfwGetTime();
        vector<ColorField> test = doRLE2(data);
        double thisTime = glfwGetTime();

//        cout<< "time spent for run time encoding: " << thisTime - lastTime << endl;

//        cout<<"array has: " << test.size() << " entries, which makes a total of ..." << endl;
//        cout<<"... size : "<< (float)(sizeof(float) * test.size() * 4)/1000000<< " MByte"<<endl;

//        float* testDecode = doRLEDecode(test);

        cout<<"-----------------------------------"<<endl;

        for(ColorField x : test){
     	   cout<< x.appearence << " times the Color: " << x.r << endl;
        }

        glBindTexture(GL_TEXTURE_2D, tex2Handle);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tWidth, tHeight, GL_RGBA, GL_FLOAT, data);
        glBindTexture(GL_TEXTURE, 0);

        pass2																			//show on a plane
        ->clear(1, 1, 1, 0)
//        ->texture("tex2", pass->get("fragColor"))
        ->texture("tex2", tex2Handle)
        ->run();

//        glBindTexture(GL_TEXTURE_2D, tex1Handle);
//        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, data);
//        glBindTexture(GL_TEXTURE_2D, 0);

    });
}
