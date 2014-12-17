#include "SlicemapUtilities.h"

// create 32bit uint bitmask
GLuint createR32UIBitMask()
{
	GLuint bitmask = 0;

	// 32 bit values, one bit set per entry
	unsigned long int bitmaskData[32];
	unsigned long int val = pow(2,31);

	// 32 bit values, one bit set per entry
	for ( int i = 0; i < 32; i++ )
	{
		bitmaskData[i] = val;
		val = val >> 1;
	}

	glGenTextures(1, &bitmask);
	glBindTexture(GL_TEXTURE_1D, bitmask);

	// allocate memory:  1D Texture,  1 level,   long uint format (32bit)
	glTexStorage1D( GL_TEXTURE_1D, 1, GL_R32UI, 32);
	glTexSubImage1D( GL_TEXTURE_1D, 0, 0, 32, GL_RED_INTEGER, GL_UNSIGNED_INT, &bitmaskData);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_1D, 0);

	return bitmask;
}
// create 32bit uint bitmask
GLuint createRGBA32UIBitMask()
{
	GLuint bitmask = 0;

	// 4 channels, 32 masks consisting of 4 values each
	unsigned int bitmaskData[4*32*4]={0};

	// per color channel R G B A
	for ( int c = 0; c < 4; c++)
	{
		unsigned int val = pow(2,31);

		// 32 bit values, one bit set per entry
		for ( int i = 0; i < 32; i++ )
		{
			bitmaskData[i * 4 + c + c * 32 * 4] = val;
			val = val >> 1;
		}
	}

//	for ( int i = 0; i < 4*32; i++)
//	{
//		std::cout << "rgba(" << i << ") : "
//		<< bitmaskData[i*4] << ", "
//		<< bitmaskData[i*4+1]<< ", "
//		<< bitmaskData[i*4+2]<< ", "
//		<< bitmaskData[i*4+3]
//		<< std::endl;
//	}

	glGenTextures(1, &bitmask);
	glBindTexture(GL_TEXTURE_1D, bitmask);

	// allocate memory:  1D Texture,  1 level,   long uint format (32bit)
	glTexStorage1D( GL_TEXTURE_1D, 1, GL_RGBA32UI, 32*4);
	glTexSubImage1D( GL_TEXTURE_1D, 0, 0, 32*4, GL_RGBA_INTEGER, GL_UNSIGNED_INT, &bitmaskData);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_1D, 0);

	return bitmask;
}


Slicemap::Slicemap(std::map<std::string, ShaderProgram::Info>* outputMap,
		int width, int height) {
	int size = outputMap->size();

		glGenFramebuffers(1, &frameBufferObjectHandle);

	    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObjectHandle);
	    std::vector<GLuint> drawBufferHandles(size);

	    for (auto e : *outputMap) {
	    	GLuint handle;
	    	glGenTextures(1, &handle);
		    glBindTexture(GL_TEXTURE_2D, handle);

//-->       // explicitly create a RGBA 32bit UI Texture

			glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32UI, width, height);
//			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_INT, NULL);

//		    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32UI, width, height, 0, GL_RGBA_INTEGER, GL_UNSIGNED_INT, 0);

		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		    GLuint currentAttachment = GL_COLOR_ATTACHMENT0 + e.second.location;
		    glFramebufferTexture2D(GL_FRAMEBUFFER, currentAttachment, GL_TEXTURE_2D, handle, 0);

	    	textureMap[e.first] = handle;
		    drawBufferHandles[e.second.location] = currentAttachment;
	    }
	    glDrawBuffers(size, &drawBufferHandles[0]);


//	    //TODO ????? investigate further to avoid in future ...
//		GLuint depthTexture;
//		glGenTextures( 1, &depthTexture);
//		glBindTexture( GL_TEXTURE_2D, depthTexture);
//		glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, width, height, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
//
//	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//
//		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

}

SlicemapRenderPass::SlicemapRenderPass(VertexArrayObject* vertexArrayObject,
		ShaderProgram* shaderProgram, int width, int height)
: RenderPass(vertexArrayObject, shaderProgram)
{
	SlicemapRenderPass::autoGenerateSlicemap(width, height);
}


void SlicemapRenderPass::autoGenerateSlicemap(int width, int height) {
	if (frameBufferObject) delete frameBufferObject;
		frameBufferObject = new Slicemap(&(shaderProgram->outputMap), width, height);
}
