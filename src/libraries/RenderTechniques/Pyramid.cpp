#include "Pyramid.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"

Pyramid::Pyramid(int width, int height, std::string pushFragmentShaderPath, std::string pullFragmentShaderPath = "") 
{
	pushShaderProgram = new ShaderProgram("/Filters/fullscreen.vert", pushFragmentShaderPath);

	if (pullFragmentShaderPath == "") {
		pullShaderProgram = NULL;
	} else {
		pullShaderProgram = new ShaderProgram("/Filters/fullscreen.vert", pullFragmentShaderPath);
	}
	vertexArrayObject = new Quad(); 

	int numTextures = pushShaderProgram->outputMap.size();
	std::vector<GLuint> textures(numTextures);
	std::vector<GLuint> drawBuffers(numTextures);

	glGenTextures(numTextures, &textures[0]);

	for (auto e : pushShaderProgram->outputMap) {
		GLuint handle = textures[e.second.location];
		glBindTexture(GL_TEXTURE_2D, handle);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glGenerateMipmap(GL_TEXTURE_2D);

		drawBuffers[e.second.location] = GL_COLOR_ATTACHMENT0 + e.second.location;
		
		pushShaderProgram->texture("pyramid_" + e.first, handle);
		if (pullShaderProgram != NULL) {
			pullShaderProgram->texture("pyramid_" + e.first, handle);
		}

		textureMap[e.first] = handle;
	}


	int mipmapNumber = (int)glm::log2(glm::max<float>(width, height));
	fboHandles.resize(mipmapNumber);
	glGenFramebuffers(mipmapNumber, &fboHandles[0]);

	for (int i = 0; i < mipmapNumber; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, fboHandles[i]);
		for (int j = 0; j < numTextures; j++) {
        	glFramebufferTexture2D(GL_FRAMEBUFFER, drawBuffers[j], GL_TEXTURE_2D, textures[j], i);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



Pyramid::~Pyramid() {

}



Pyramid* Pyramid::push() {
	pushShaderProgram->use();
	for (int level = 0; level < getMipmapNumber(); level++) {
		glBindFramebuffer(GL_FRAMEBUFFER, fboHandles[level]);

		pushShaderProgram->update("level", level);
		vertexArrayObject->draw();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return this;
}



Pyramid* Pyramid::pull() {
	if (pullShaderProgram == NULL) {
		return this;
	}

	pullShaderProgram->use();
	for (int level = getMipmapNumber() - 1; level >= 0; level--) {
		glBindFramebuffer(GL_FRAMEBUFFER, fboHandles[level]);

		pullShaderProgram->update("level", level);
		vertexArrayObject->draw();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return this;
}



Pyramid* Pyramid::run() {
	push();
	pull();

	return this;
}



Pyramid* Pyramid::texture(std::string name, GLuint textureHandle) {
	pushShaderProgram->texture(name, textureHandle);
	if (pullShaderProgram != NULL) {
		pullShaderProgram->texture(name, textureHandle);
	}
	return this;
}



Pyramid* Pyramid::clear(float r, float g, float b, float a, int level) {
	glClearColor(r, g, b, a);
	
	if (level == -1) {
		for (auto f : fboHandles) {
			glBindFramebuffer(GL_FRAMEBUFFER, f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	} else {	
		glBindFramebuffer(GL_FRAMEBUFFER, fboHandles[level]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	glClearColor(0, 0, 0, 0);
	return this;
}



Pyramid* Pyramid::clear(int level) {
	clear(0, 0, 0, 0, level);
	return this;
}



GLuint Pyramid::get(std::string name) {
	return textureMap[name];
}



int Pyramid::getMipmapNumber() {
	return fboHandles.size();
}