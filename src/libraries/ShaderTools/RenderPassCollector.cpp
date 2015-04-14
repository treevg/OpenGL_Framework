#include "RenderPassCollector.h"
#include <math.h>


RenderPassCollector::RenderPassCollector(vector<VertexArrayObject*> objects, ShaderProgram* shaderProgram) 
 : objects(objects), shaderProgram(shaderProgram), frameBufferObject(0)
{
   frameBufferObject = new FrameBufferObject();
   
}

RenderPassCollector::RenderPassCollector(vector<VertexArrayObject*> objects, ShaderProgram* shaderProgram, int width, int height ) 
 : objects(objects), shaderProgram(shaderProgram), frameBufferObject(0)
{
   autoGenerateFrameBufferObject(width, height);
   
}

	

void RenderPassCollector::run() {
	if(frameBufferObject->getFrameBufferObjectHandle()!=0){

		 cout << " -------------------Rendered to texture in the location  " << frameBufferObject->getFrameBufferObjectHandle() << endl;
	}
  	frameBufferObject->bind();

	     for ( int i=0; i < this->objects.size(); i++ ){
	        	shaderProgram->use();
	      	    shaderProgram->update("uniformModel", objects[i]->modelMatrix);
	      	    cout << "color is " << objects[i]->color.x<< " " << objects[i]->color.y << " " << objects[i]->color.y << endl;
	      	    
	      	    if(objects[i]->color.x>-1){
	      	    	shaderProgram->update("color", objects[i]->color);}

	      	    if(objects[i]->textureHandle!= -1){
                    shaderProgram->texture("diffuse_text", objects[i]->textureHandle);
	      	    }
	        	objects[i]->draw();
	        	
	      }

       glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



 FrameBufferObject* RenderPassCollector::getFrameBufferObject() const{
   return this->frameBufferObject;
 }

 RenderPassCollector* RenderPassCollector::clear(float r, float g, float b, float a) {
		frameBufferObject->clear(r, g, b, a);
		return this;
	}

	GLuint RenderPassCollector::get(std::string name) {
		cout << "location of " << name  << " is " << frameBufferObject->get(name) << endl;
	return frameBufferObject->get(name);
}

void RenderPassCollector::autoGenerateFrameBufferObject(int width, int height) {
	if (frameBufferObject) delete frameBufferObject;
	   frameBufferObject = new FrameBufferObject(&(shaderProgram->outputMap), width, height);
}

GLuint  RenderPassCollector::getFrameBufferHandle(){
     return frameBufferObject->getFrameBufferObjectHandle();

}