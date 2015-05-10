#include "ShaderTools/DefaultRenderLoop.h"
#include "ShaderTools/RenderPass.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "ShaderTools/VertexArrayObjects/Cube.h"
#include "Compression/TextureTools.h"
#include "Compression/ComputeShaderTools.h"
#include "Compression/ColorField.h"
#include <fstream>
#include <sstream>
#include <glm/gtx/string_cast.hpp>

using namespace std;
using namespace glm;

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *------------------------------------------------------variable declaration--------------------------------------------------------------------------
 -------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

//auto sp = new ShaderProgram({"/Compression/test1.vert", "/Compression/test1.frag"});
//auto pass = new RenderPass(new Cube(), sp, width, height);
 auto sp = new ShaderProgram({"/Filters/fullscreen.vert", "/Filters/toneMapperLinear.frag"});
 auto pass = new RenderPass(new Quad(), sp, width, height);
//auto pass = new RenderPass(new Cube(), sp);

auto compositingSP = new ShaderProgram({"/Compression/pass.vert", "/Compression/compositing.frag"});

auto pass2 = new RenderPass(new Quad(), compositingSP);

auto RGBtoYCbCr = new ShaderProgram(GL_COMPUTE_SHADER, "/Compression/rgbToYCbCr.comp");
auto YCbCrToRGB = new ShaderProgram(GL_COMPUTE_SHADER, "/Compression/YCbCrToRGB.comp");
auto compressCbCr = new ShaderProgram(GL_COMPUTE_SHADER, "/Compression/compressCbCr.comp");
auto compressedYCbCrToRGB = new ShaderProgram(GL_COMPUTE_SHADER, "/Compression/compressedYCbCrToRGB.comp");
auto dct = new ShaderProgram(GL_COMPUTE_SHADER, "/Compression/dct.comp");
auto dct2 = new ShaderProgram(GL_COMPUTE_SHADER, "/Compression/dct2.comp");
auto dct3 = new ShaderProgram(GL_COMPUTE_SHADER, "/Compression/dct3.comp");
auto pseudoQuantize = new ShaderProgram(GL_COMPUTE_SHADER, "/Compression/quantize.comp");
auto pseudoDequantize = new ShaderProgram(GL_COMPUTE_SHADER, "/Compression/dequantize.comp");
auto idct2 = new ShaderProgram(GL_COMPUTE_SHADER, "/Compression/idct2.comp");
auto idct3 = new ShaderProgram(GL_COMPUTE_SHADER, "/Compression/idct3.comp");
auto splitChannels = new ShaderProgram(GL_COMPUTE_SHADER, "/Compression/splitChannels.comp");
auto split2Channels = new ShaderProgram(GL_COMPUTE_SHADER, "/Compression/split2Channels.comp");
auto mergeChannels = new ShaderProgram(GL_COMPUTE_SHADER, "/Compression/mergeChannels.comp");
auto merge2Channels = new ShaderProgram(GL_COMPUTE_SHADER, "/Compression/merge2Channels.comp");
auto fZigZag = new ShaderProgram(GL_COMPUTE_SHADER, "/Compression/fZigZag.comp");
auto rZigZag = new ShaderProgram(GL_COMPUTE_SHADER, "/Compression/rZigZag.comp");

float cubeAngle = 0.0f;
float rotationSpeed = 0.01f;

glm::mat4 viewMat       = glm::lookAt(
    glm::vec3(0,5,4), // Camera is at (0,10,10), in World Space
    glm::vec3(0,0,0), // and looks at the origin
    glm::vec3(0,1,0));  // Head is up (set to 0, 1 ,0 to look upside-down)

using namespace glm;

glm::mat4 projMat = glm::perspective(45.0f, float(width)/float(height), 0.1f, 100.0f);
mat4 cubeModel = translate(mat4(1.0f), vec3(0.0f, 1.0f, 0.0f));

GLuint textureHandle = TextureTools::loadTexture(RESOURCES_PATH "/jpg/cubeTexture.jpg");
GLuint bambus = TextureTools::loadTexture(RESOURCES_PATH "/jpg/cubeTexture.jpg");

GLuint tex1Handle;
GLuint tex2Handle;
GLuint tex3Handle;
GLuint tex4Handle;
GLuint tex5Handle;
GLuint tex52Handle;
GLuint tex6Handle;
GLuint tex7Handle;
GLuint tex8Handle;
GLuint tex9Handle;
GLuint texQuantHandle;
GLuint texQuant2Handle;
GLuint texFinalHandle;
GLuint frameBufferObjectHandle;

double mouseX, mouseY;																	//stubs for mouse coordinates
vector<float> pixelColor(4);																//container for color of texture at mouse coordinates

int tWidth, tHeight;																	//stub for dimensions of texture in CPU-Memory
float* data;																			//container for texture in CPU-Memory as plain array
float* data2;

int print = 0;

vector<float> values;
vector<int> counts;

double oldTime, newTime;

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *------------------------------------------------------function declaration--------------------------------------------------------------------------
 -------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

void writeFloatToFile(float* array, string fileName, int numValues){
	ofstream outFile(fileName, ios::binary);
	if(outFile.fail()){
		cout<<"ERRORRRRRRRRRRRRRRRRRRRRRRRRRRR"<< endl;
	}
	outFile.write((char*) array, sizeof(float)*numValues);
	outFile.close();

}

void writeIntToFile(int* array, string fileName, int numValues){
	ofstream outFile(fileName, ios::binary);
	if(outFile.fail()){
		cout<<"ERRORRRRRRRRRRRRRRRRRRRRRRRRRRR"<< endl;
	}
	outFile.write((char*) array, sizeof(int)*numValues);
	outFile.close();
}

int lengthOfFile(string fileName){
	ifstream inFile(fileName, ios::binary);
	inFile.seekg (0, inFile.end);
	int length = inFile.tellg();
	inFile.close();
	return length;
}

void readFloatFromFile(string fileName, float* array){
	ifstream inFile(fileName, ios::binary);
	if(inFile.fail())
		cout<<"Error!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<< endl;

	 inFile.seekg (0, inFile.end);
	 int length = inFile.tellg();
	 inFile.seekg (0, inFile.beg);

	inFile.read((char*) array, length);
	inFile.close();
}

void readIntFromFile(string fileName, int* array){
	ifstream inFile(fileName, ios::binary);
	if(inFile.fail())
		cout<<"Error!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<< endl;

	 inFile.seekg (0, inFile.end);
	 int length = inFile.tellg();
	 inFile.seekg (0, inFile.beg);

	inFile.read((char*) array, length);
	inFile.close();
}

void doRLE2(float *array, vector<float>* outValue, vector<int>* outCounts){
	float colorOld = array[0];
	float color;
	int count = 1;
	int countGlobal = 0;


//	for(int i = 4; i < (tWidth * tHeight * 4) ; i+=12){
	for(int i = 1; i < (tWidth * tHeight) ; i++){
//		countGlobal++;

//		if (array[i] != 0)
//			cout<<"Pos: "<< i << endl;

		color = array[i];

		if(glm::abs(colorOld - color) >= 0.01){
			outValue->push_back(colorOld);
			outCounts->push_back(count);
			colorOld = color;

//			cout<<"count: "<< count << endl;
			count = 1;
		}

		else{
			count++;
		}
	}


	outValue->push_back(colorOld);
	outCounts->push_back(count);

//	if(print == 10){
////	writeFloatToFile(array, "original", tWidth*tHeight);
//
//		int countings[outCounts->size()];
//		float valuess[outValue->size()];
//
//		for(int i = 0; i < outCounts->size(); i++){
//			countings[i] = outCounts->at(i);
//			valuess[i] = outValue->at(i);
//		}
//
//		writeIntToFile(countings, "counts.dat", outCounts->size());
//		writeFloatToFile(valuess, "values.dat", outValue->size());
//	}

//	cout<<"Count = "<< count <<"."<< "value = " << colorOld <<endl;
//	cout<< "Array has : " << countGlobal << endl;

}

void doRLEDecode3(float* array){
	int counts[lengthOfFile("counts.dat")/4];

	readIntFromFile("counts.dat", counts);

	float values[lengthOfFile("values.dat")/4];
	readFloatFromFile("values.dat", values);

	int i,j,k = 0;

	int length = sizeof(counts)/sizeof(counts[0]);

	for(i = 0 ; i < length ; i++){
		for(j = 0; j < counts[i]; j ++){
			array[k+j] = values[i];
		}
		k+=j;
	}
}


//void doRLEDecode3(){
//	int counts[lengthOfFile("counts.dat")/4];
//
//	readIntFromFile("counts.dat", counts);
//
//	cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!length of counts is: "<< sizeof(counts)/sizeof(counts[0]) << endl;
//
//	float values[lengthOfFile("values.dat")/4];
//	readFloatFromFile("values.dat", values);
//
//	int i,j,k = 0;
//
//	int length = sizeof(counts)/sizeof(counts[0]);
//
//	for(i = 0 ; i < length ; i++){
//		for(j = 0; j < counts[i]; j ++){
//			array[k+j] = values[i];
//		}
//		k+=j;
//	}
//
//
//}

void doRLEDecode3(vector<float>* value, vector<int>* counts, float* array){

	int i,j,k = 0;

	for(i = 0 ; i < counts->size(); i++){
		for(j = 0; j < counts->at(i); j ++){
			array[k+j] = value->at(i);
		}
		k+=j;
	}

	cout<<"values<float> size is: " << value->size()<< ", wich makes a total of: " << sizeof(float) * value->size() << endl;
	cout<<"counts<int> size is: " << counts->size() << ", wich makes a total of: " << sizeof(short) * counts->size() << endl;
	cout<<"total is: " << k << endl;

//	int i,j;
//
//	for(i = 0 ; i < tWidth * tHeight; i++){
//		for(j = 0; j < counts->front(); j++){
//			array[i +j] = value->front();
//		}
//		i+=j;
//		counts->pop_back();
//		value->pop_back();
//	}
//
//	cout<< "i = " << i << endl;
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
			// std::cout << "Frames per second: " + glm::to_string(fps) << endl;
		}
		frames = 0.0;
		tZero = glfwGetTime();
	}

	return fps;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *------------------------------------------------------end of declaration--------------------------------------------------------------------------
 -------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

int main(int argc, char *argv[]) {
    glGenFramebuffers(1, &frameBufferObjectHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObjectHandle);

    glGenTextures(1, &tex1Handle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex1Handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex1Handle, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    glGenTextures(1, &tex2Handle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex2Handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, NULL);
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

    glGenTextures(1, &tex5Handle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex5Handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, tex5Handle, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT4);

    glGenTextures(1, &tex52Handle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex52Handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, NULL);


    glGenTextures(1, &tex6Handle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex6Handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, tex6Handle, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT5);

    glGenTextures(1, &tex7Handle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex7Handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, width/8*64, height/8, 0, GL_RED, GL_FLOAT, NULL);

    glGenTextures(1, &tex9Handle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex9Handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, width/8*64, height/8, 0, GL_RED, GL_FLOAT, NULL);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT6, GL_TEXTURE_2D, tex7Handle, 0);
//    glDrawBuffer(GL_COLOR_ATTACHMENT6);

    glGenTextures(1, &tex8Handle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex8Handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT7, GL_TEXTURE_2D, tex8Handle, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT7);

    glGenTextures(1, &texQuantHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texQuantHandle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, width, height, 0, GL_RED, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT7, GL_TEXTURE_2D, texQuantHandle, 0);
//    glDrawBuffer(GL_COLOR_ATTACHMENT8);

    glGenTextures(1, &texQuant2Handle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texQuantHandle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, width, height, 0, GL_RED, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT7, GL_TEXTURE_2D, texQuant2Handle, 0);

    glGenTextures(1, &texFinalHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texFinalHandle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texFinalHandle, 0);
//    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObjectHandle);
    GLfloat clearColor[4] = {1, 1, 1, 0};
    glClearBufferfv(GL_COLOR, 0, clearColor);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindTexture(GL_TEXTURE_2D, tex7Handle);																	//prepare swapping Texture between Memories
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &tWidth);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &tHeight);
    std::cout<<"width: "<< tWidth <<", height: " << tHeight << std::endl;

    data = (float*)malloc( sizeof(float) * tHeight * tWidth);
    data2 = (float*)malloc( sizeof(float) * tHeight * tWidth);
//    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, data);														//this is where the swapping magic happens

//    cout<<"array has: " << tHeight * tWidth << " entries, which makes a total of ..." << endl;
//    cout<<"... size : "<< sizeof(float) * tHeight * tWidth << " MByte"<<endl;
    glBindTexture(GL_TEXTURE_2D, 0);																			//end preparation --> image is swapped and stored in "data"

    /*-------------------------------------------------------------------------------------------------------------------------------------------------------------------
     *------------------------------------------------------RENDER LOOP--------------------------------------------------------------------------
     -------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

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
        print++;

        pass
        -> clear(1, 1, 1, 0)
//        -> update("uniformView", viewMat)
//        -> update("uniformProjection", projMat)
//        -> update("uniformModel", cubeModel)
        -> texture("tex", bambus)
        -> run();

        RGBtoYCbCr->use();
        glBindImageTexture(0, pass->get("fragColor"), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);		//INPUT texture
//        glBindImageTexture(0, bambus, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
        glBindImageTexture(1, tex1Handle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);				//OUTPUT texture
        glDispatchCompute(int(width/16), int(height/16), 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        compressCbCr->use();
        glBindImageTexture(0, tex1Handle, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);					//INPUT texture
        glBindImageTexture(1, tex3Handle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RG32F);					//OUTPUT texture1  Chroma-Channels (Cb, Cr)
        glBindImageTexture(2, tex4Handle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RG32F);					//OUTPUT texture2  Brightness-Channel (Y) and Depth-Channel/transperancy (A)
        glDispatchCompute(int(width/16), int(height/16), 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        split2Channels->use();
        glBindImageTexture(0, tex4Handle, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RG32F);					//INPUT texture
        glBindImageTexture(1, tex5Handle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);					//OUTPUT texture1  (Y)Channel
        glBindImageTexture(2, tex2Handle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);					//OUTPUT texture2  (A)Channel
        glDispatchCompute(int(width/8), int(height/8), 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	        dct2->use();
	        glBindImageTexture(0, tex5Handle, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);					//INPUT texture
	        glBindImageTexture(1, tex6Handle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);					//OUTPUT texture1  Chroma-Channels (Cb, Cr)
	        glDispatchCompute(int(width/8), height, 1);
	        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	        dct3->use();
	        glBindImageTexture(0, tex6Handle, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);					//INPUT texture
	        glBindImageTexture(1, tex5Handle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);					//OUTPUT texture1  Chroma-Channels (Cb, Cr)
	        glDispatchCompute(width, int(height/8), 1);
	        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	        pseudoQuantize->use();
	        glBindImageTexture(0, tex5Handle, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);					//INPUT texture
	        glBindImageTexture(1, texQuantHandle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R16F);					//OUTPUT texture1  Chroma-Channels (Cb, Cr)
	        glDispatchCompute(int(width/8), int(height/8), 1);
	        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	        fZigZag->use();
	        glBindImageTexture(0, texQuantHandle, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R16F);					//INPUT texture
	        glBindImageTexture(1, tex7Handle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R16F);					//OUTPUT texture1  Chroma-Channels (Cb, Cr)
	        glDispatchCompute(int(width/8), int(height/8), 1);
	        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	        glBindTexture(GL_TEXTURE_2D, tex7Handle);
	        glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, data);
	        glBindTexture(GL_TEXTURE_2D, 0);
////
////	        vector<float>* val = &values;
////	        vector<int>* coun = &counts;
////
////	        doRLE2(data, val, coun);
////
	        doRLEDecode3(data2);
////	        doRLEDecode3(val, coun, data2);
//
////	        values.clear();
////	        counts.clear();
//
             glBindTexture(GL_TEXTURE_2D, tex9Handle);
             glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tWidth, tHeight, GL_RED, GL_FLOAT, data2);
             glBindTexture(GL_TEXTURE, 0);

//        cout<< "time spent for run time encoding: " << thisTime - lastTime << endl;
//
//        cout<<"array has: " << test.size() << " entries, which makes a total of ..." << endl;
//        cout<<"... size : "<< (float)(sizeof(float) * test.size() * 4)/1000000<< " MByte"<<endl;

//	        rZigZag->use();
//	        glBindImageTexture(0, tex9Handle, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R16F);					//INPUT texture
//	        glBindImageTexture(1, texQuant2Handle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R16F);					//OUTPUT texture1  Chroma-Channels (Cb, Cr)
//	        glDispatchCompute(int(width*8/64), int(height/8), 1);
//	        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
//
//	        pseudoDequantize->use();
//	        glBindImageTexture(0, texQuant2Handle, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R16F);					//INPUT texture
//	        glBindImageTexture(1, tex52Handle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);					//OUTPUT texture1  Chroma-Channels (Cb, Cr)
//	        glDispatchCompute(int(width/8), int(height/8), 1);
//	        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

//	        idct2->use();
//	        glBindImageTexture(0, tex5Handle, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);					//INPUT texture
//	        glBindImageTexture(1, tex6Handle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);					//OUTPUT texture1  Chroma-Channels (Cb, Cr)
//	        glDispatchCompute(int(width/8), height, 1);
//	        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
//
//	        idct3->use();
//	        glBindImageTexture(0, tex6Handle, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);					//INPUT texture
//	        glBindImageTexture(1, tex8Handle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);					//OUTPUT texture1  Chroma-Channels (Cb, Cr)
//	        glDispatchCompute(width, int(height/8), 1);
//	        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);


//        merge2Channels->use();
////        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
//			glBindImageTexture(0, tex8Handle, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
////        else
////        	glBindImageTexture(0, tex5Handle, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
//        glBindImageTexture(1, tex2Handle, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
//        glBindImageTexture(2, tex4Handle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RG32F);
//        glDispatchCompute(int(width/8), int(height/8), 1);
//        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	     mergeChannels->use();
	     glBindImageTexture(0, tex9Handle, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R16F);
	     glBindImageTexture(1, texFinalHandle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	     glDispatchCompute(int(width/8), int(height/8), 1);
	     glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);



//        YCbCrToRGB->use();
//        glBindImageTexture(0, tex1Handle, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
//        glBindImageTexture(1, tex2Handle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
//        glDispatchCompute(int(width/16), int(height/16), 1);
//        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

//        compressedYCbCrToRGB->use();
//        glBindImageTexture(0, tex3Handle, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RG32F);					//INPUT texture1  Chroma-Channels (Cb, Cr)
//        glBindImageTexture(1, tex4Handle, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RG32F);					//INPUT texture2  Brightness-Channel (Y) and Depth-Channel (A)
//        glBindImageTexture(2, texFinalHandle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);				//OUTPUT texture RGBA
//        glDispatchCompute(int(width/16), int(height/16), 1);
//        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);



//        glBindTexture(GL_TEXTURE_2D, tex2Handle);
//        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tWidth, tHeight, GL_RGBA, GL_FLOAT, data2);
//        glBindTexture(GL_TEXTURE, 0);

        pass2																			//show on a plane
        ->clear(1, 1, 1, 0)
//        ->texture("tex2", pass->get("fragColor"))
//        ->texture("tex2", tex1Handle)
        ->texture("tex2", texFinalHandle)
        ->run();
//        int wiggle;
//        glBindTexture(GL_TEXTURE_2D, tex3Handle);
//        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &wiggle);
//        glBindTexture(GL_TEXTURE_2D, 0);
//        cout << "width: " << wiggle << endl;

//        glBindTexture(GL_TEXTURE_2D, tex1Handle);
//        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, data);
//        glBindTexture(GL_TEXTURE_2D, 0);

//        cout<<"-----------------------------------"<<endl;
    });
}
