#include "HierarchicalHoleFilling.h"

HierarchicalHoleFilling::HierarchicalHoleFilling(
	glm::vec2 resolution, 
	VertexArrayObject* vertexArrayObject, 
	int mipmapNumber,
	std::string vertexShaderPath,
	std::string fillFragmentPath,
	std::string reduceFragmentShaderPath)
	: vertexArrayObject(vertexArrayObject)
{
	if (vertexArrayObject == NULL) this->vertexArrayObject = new Quad();
	mipmapNumber = glm::clamp(mipmapNumber, 1, (int)glm::log2(glm::max<float>(resolution.x, resolution.y)));
	this->mipmapNumber = mipmapNumber;

    glGenTextures(1, &mipmap);
    glBindTexture(GL_TEXTURE_2D, mipmap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, resolution.x, resolution.y, 0, GL_RGBA, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glGenerateMipmap(GL_TEXTURE_2D);

    GLuint* mipmapFBOHandles = new GLuint[mipmapNumber];
    glGenFramebuffers(mipmapNumber, mipmapFBOHandles);

    for (int i = 0; i < mipmapNumber; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, (mipmapFBOHandles)[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mipmap, i);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);

        auto fbo = new FrameBufferObject();
        fbo->setFrameBufferObjectHandle(mipmapFBOHandles[i]);
        mipmapFBOs.push_back(fbo);
    }

    // delete mipmapFBOHandles;

    reduce = (new RenderPass(
        this->vertexArrayObject, 
        new ShaderProgram(vertexShaderPath, reduceFragmentShaderPath)))
    		->update("resolution", resolution);

    fill = (new RenderPass(
        this->vertexArrayObject, 
        new ShaderProgram(vertexShaderPath, fillFragmentPath)))
            ->texture("mipmapTexture", mipmap)
            ->update("resolution", resolution);

}

HierarchicalHoleFilling::~HierarchicalHoleFilling(){
	//TODO
}

HierarchicalHoleFilling* HierarchicalHoleFilling::texture(GLuint input){
	this->inputTexture = input;
	return this;
}

HierarchicalHoleFilling* HierarchicalHoleFilling::run(){
	reduce->texture("mipmapTexture", inputTexture);

    for (int i = 0; i < mipmapNumber; i++) {
        reduce
        	->update("level", i)
        	->setFrameBufferObject(mipmapFBOs[i])
            ->run();
        if (i == 0) {
        	reduce->texture("mipmapTexture", mipmap);
        }
    }

    for (int i = mipmapNumber-1; i >= 0; i--) {
        fill
        	->update("level", i)
        	->setFrameBufferObject(mipmapFBOs[i])
        	->run();
    }
	return this;
}

GLuint HierarchicalHoleFilling::getOutput() {
	return mipmap;
}

int HierarchicalHoleFilling::getMipmapNumber() {
	return mipmapNumber;
}