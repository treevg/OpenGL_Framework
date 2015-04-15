#include "RenderPassCollector.h"
#include <cmath>


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
	      cout <<  " texture handle is "  << objects[i]->textureHandle << endl; 
	      	    
	      	   /* if(objects[i]->color.x>-1){
	      	    	shaderProgram->update("color", objects[i]->color);}*/ 

	      	    if(objects[i]->textureHandle!= -1){
                    shaderProgram->texture("diffuse_text", objects[i]->textureHandle);
	      	    }

	      	        if (this->objects[i]->getTextures().size() > 0) {

                      for (MeshTexture tex: this->objects[i]->getTextures()){
        
                       cout <<  " ------  DEBUG: TEXTURE ID ------ "  << tex.id << endl; 
     
                            switch( tex.type ) { 

  	                    case 'a': 

                    	shaderProgram->textureModel("ambient_text", tex.id); 

                     	case 'd': 

                    	shaderProgram->textureModel("diffuse_text", tex.id); 

                        case 's': 

                         shaderProgram->textureModel("specular_text", tex.id); 

                 //       case 'e': 

                     //    shaderProgram->textureModel("emission_text", tex.id); 

                    }
    	          }

         }
                       //TODO: check somehow if struct is set
                       /* if(objects[i]->material!=NULL){

                          shaderProgram->update("light_diffuse", objects[i]->material.color_dif);

                        }
                        */

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

RenderPassCollector* RenderPassCollector::setFrameBufferObject(FrameBufferObject* frameBufferObject) {
        this->frameBufferObject = frameBufferObject;
     return this;
}


void RenderPassCollector::addVAOS(vector<VertexArrayObject*> moreObjects) {


this->objects.insert(this->objects.end(),moreObjects.begin(), moreObjects.end());

}


void RenderPassCollector::addVertexArrayObject( VertexArrayObject* obj) {


this->objects.push_back(obj);

}