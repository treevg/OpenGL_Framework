#ifndef OPENGLTESTUTILITIES_H_
#define OPENGLTESTUTILITIES_H_

#include <GLFW/glfw3.h>
#include <GL/glew.h>

void testZeros(GLuint texture, GLenum format = GL_RGBA)
{
	glActiveTexture( GL_TEXTURE10 );
	glBindTexture(GL_TEXTURE_2D, texture);
	int width_, height_;
	glGetTexLevelParameteriv(GL_TEXTURE_2D,0, GL_TEXTURE_WIDTH, &width_);
	glGetTexLevelParameteriv(GL_TEXTURE_2D,0, GL_TEXTURE_HEIGHT, &height_);
//	std::cout<<"2D texture: "<< texture <<" width: "<< width_ <<", height: "<< height_ << std::endl;

	unsigned int *data = (unsigned int*)malloc( sizeof(unsigned int) * height_ * width_ * 4);
	glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_INT, data);

	bool notzero = false;
	for( unsigned int i = 0; i < width_ * height_ * 4 ; i++ )
	{
		if ( data[i] > 0 )
		{
			std::cout<<"i[" << i / 4 << ", " << i % 4 << "] : " << data[i] << std::endl;
			notzero = true;
			break;
		}
	}
	if ( !notzero )
	{
		std::cout << "2D texture: " << texture <<" is all zeros..." << std::endl;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture( GL_TEXTURE0 );

	delete data;
}

void testZeros1D(GLuint texture)
{
	glActiveTexture( GL_TEXTURE11 );
	glBindTexture(GL_TEXTURE_1D, texture);
	int width_ = 0;
	glGetTexLevelParameteriv(GL_TEXTURE_1D, 0, GL_TEXTURE_WIDTH, &width_);
//	std::cout<<"1D texture: " << texture << " width: "<< width_ << std::endl;

	unsigned int *data = (unsigned int*)malloc(sizeof(unsigned int) * width_ * 4);
	glGetTexImage(GL_TEXTURE_1D, 0, GL_RGBA_INTEGER, GL_UNSIGNED_INT, data);

	bool notzero = false;
	for( unsigned int i = 0; i < width_*4; i++ )
	{
		if ( data[i] > 0 )
		{
			std::cout<<"i[" << i / 4 << ", " << i % 4 <<"] : " << data[i] << std::endl;
			notzero = true;
		}
	}
	if ( !notzero )
	{
		std::cout << "1D texture: " << texture <<" is all zeros..." << std::endl;
	}

	delete data;

	glBindTexture(GL_TEXTURE_1D, 0);
	glActiveTexture( GL_TEXTURE0 );
}

bool once = false;

void testError()
{
    GLenum error = glGetError();
	switch (error) {
	case GL_NO_ERROR:
		std::cout << "Status : everything's fine up until now" << std::endl;
		break;
	default:
		std::cout << "ERROR-Status ";
		switch(error)
		{
		   case GL_INVALID_ENUM:
		    	std::cout<<"--- INVALID_ENUM"<< std::endl;
		    break;
		    case GL_INVALID_VALUE:
		    	std::cout<<"--- INVALID_VALUE"<< std::endl;
		    break;
		    case GL_INVALID_OPERATION:
		    	std::cout<<"--- INVALID_OPERATION"<< std::endl;
		    break;
		    case GL_INVALID_FRAMEBUFFER_OPERATION:
		    	std::cout<<"--- INVALID_FRAMEBUFFER_OPERATION"<< std::endl;
		    break;
		    case GL_OUT_OF_MEMORY:
		    	std::cout<<"--- OUT_OF_MEMORY"<< std::endl;
		    break;
		    case GL_STACK_UNDERFLOW:
		    	std::cout<<"--- STACK_UNDERFLOW"<< std::endl;
		    break;
		    case GL_STACK_OVERFLOW:
		    	std::cout<<"--- STACK_OVERFLOW"<< std::endl;
		    break;
		    default:
		    	std::cout<<"--- whaaaaa...??????????????"<<std::endl;
		}
		break;
	}
}

void testFramebuffer(GLenum target)
{
	const char* framebuffername;
	switch(target)
	{
		case GL_DRAW_FRAMEBUFFER:
			framebuffername = "DRAW_FRAMEBUFFER";
		break;
		case GL_READ_FRAMEBUFFER:
			framebuffername = "READ_FRAMEBUFFER";
		break;
		case GL_FRAMEBUFFER:
			framebuffername = "FRAMEBUFFER";
		break;
	}

	GLenum error = glCheckFramebufferStatus(target);
	switch(error)
	{
	case GL_FRAMEBUFFER_COMPLETE:
		std::cout << "FB Status : "<< framebuffername<< " is fine up until now" << std::endl;
		break;
	default:
		std::cout<<"ERROR-FB Status : " << framebuffername << " ";
		switch(error)
		{
			case GL_FRAMEBUFFER_UNDEFINED:
				std::cout<<"--- FRAMEBUFFER_UNDEFINED" << std::endl;
			break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				std::cout<<"--- FRAMEBUFFER_INCOMPLETE_ATTACHMENT" << std::endl;
			break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				std::cout<<"--- FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" << std::endl;
			break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
				std::cout<<"--- FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" << std::endl;
			break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
				std::cout<<"--- FRAMEBUFFER_INCOMPLETE_READ_BUFFER" << std::endl;
			break;
			case GL_FRAMEBUFFER_UNSUPPORTED:
				std::cout<<"--- FRAMEBUFFER_UNSUPPORTED" << std::endl;
			break;
			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
				std::cout<<"--- FRAMEBUFFER_INCOMPLETE_MULTISAMPLE" << std::endl;
			break;
			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
				std::cout<<"--- FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS" << std::endl;
			break;
			default:
				std::cout<<"--- whaaaaa... ??????????" << std::endl;
		}
	}
}

void printImageBindings()
{
	GLint image = 0;
	std::cout <<"IMAGE_BINDING_NAME:";
	for (int i = 0; i < 8; i++)
	{
		glGetIntegeri_v(GL_IMAGE_BINDING_NAME, i, &image);
		std::cout << " ["<<i<<"] : "<< image;
	}
	std::cout<<std::endl;
}

GLuint queryID[2];

void startTime() {
	// generate OpenGL query objects
	glGenQueries(2, &queryID[0] );

	// request current time-stamp
	glQueryCounter(queryID[0], GL_TIMESTAMP);
}

void stopTime() {
	// request current time-stamp
	glQueryCounter(queryID[1], GL_TIMESTAMP);

	// wait for query to become available
	unsigned int stopTimerAvailable = 0;
	while (!stopTimerAvailable )
	{
	    glGetQueryObjectuiv(queryID[1],
	    		GL_QUERY_RESULT_AVAILABLE,
	    		&stopTimerAvailable);
	}

	GLuint64 startTime, stopTime;
	double executionTime;

	// retrieve query results
	glGetQueryObjectui64v(queryID[0], GL_QUERY_RESULT, &startTime);
	glGetQueryObjectui64v(queryID[1], GL_QUERY_RESULT, &stopTime);

	// compute execution time
	executionTime = (stopTime - startTime) / 1000000.0;

	std::cout << executionTime << " (ms)" << std::endl;
}

#endif
