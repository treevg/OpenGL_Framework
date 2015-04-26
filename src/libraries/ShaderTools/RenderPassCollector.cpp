#include "RenderPassCollector.h"
#include "AssetTools/Texture.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "WarppingGame/CubemapTexture/CubemapTexture.h"
#include "ShaderTools/VertexArrayObjects/Skybox2.h"
#include <cmath>



RenderPassCollector::RenderPassCollector(vector<VertexArrayObject*> objects, map<string, ShaderProgram*> shaderPrograms) 
 : objects(objects), shaderProgramms(shaderPrograms), frameBufferObject(0)
{
   frameBufferObject = new FrameBufferObject();
   
}

RenderPassCollector::RenderPassCollector(vector<VertexArrayObject*> objects, map<string, ShaderProgram*> shaderPrograms, int width, int height ) 
 : objects(objects), shaderProgramms(shaderPrograms), frameBufferObject(0)
{


   autoGenerateFrameBufferObject(width, height);
   
}

	

void RenderPassCollector::run() {
	
	    
        frameBufferObject->bind();
        frameBufferObject->clear();
  

       for ( int i=0; i < this->objects.size(); i++ ){
              string sp = objects[i]->shaderProgramKey;
         
           	shaderProgramms[sp]->use();
	      	  shaderProgramms[sp]->update("uniformModel", objects[i]->modelMatrix);
	         
       
	   

	      	    if(objects[i]->textureHandle!= -1){
                    shaderProgramms[sp]->texture("diffuse_text", objects[i]->textureHandle);
	      	    }

	      	        if (this->objects[i]->getTextures().size() > 0) {

                      for (MeshTexture tex: this->objects[i]->getTextures()){
        
     
                      switch( tex.type ) { 

  	                  case 'a': 

                    	shaderProgramms[sp]->textureModel("ambient_text", tex.id); 

                     	case 'd': 

                    	shaderProgramms[sp]->textureModel("diffuse_text", tex.id); 

                       case 's': 

                      shaderProgramms[sp]->textureModel("specular_text", tex.id); 

                 

                    }
    	          }

         }
                              
                shaderProgramms[sp]->update("Ka", objects[i]->getMaterial().ka);
                shaderProgramms[sp]->update("Ks", objects[i]->getMaterial().ks);
                shaderProgramms[sp]->update("Kd", objects[i]->getMaterial().kd);
      

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
  std::map<std::string, ShaderProgram::Info> output;

for ( const auto &p : this->shaderProgramms )
{
  std::map<std::string, ShaderProgram::Info> om = p.second->outputMap;
  output.insert(om.begin(), om.end()); 
}


	if (frameBufferObject) delete frameBufferObject;
	   frameBufferObject = new FrameBufferObject(&(output), width, height);
}

GLuint  RenderPassCollector::getFrameBufferHandle(){
     return frameBufferObject->getFrameBufferObjectHandle();

}

RenderPassCollector* RenderPassCollector::setFrameBufferObject(FrameBufferObject* frameBufferObject) {
        this->frameBufferObject = frameBufferObject;
     return this;
}


RenderPassCollector*  RenderPassCollector::addVAOS(vector<VertexArrayObject*> moreObjects) {


this->objects.insert(this->objects.end(),moreObjects.begin(), moreObjects.end());
return this;


}


RenderPassCollector*  RenderPassCollector::addVertexArrayObject( VertexArrayObject* obj) {


this->objects.push_back(obj);
return this;


}


   RenderPassCollector::~RenderPassCollector(){

          for(VertexArrayObject* v: this->objects){

            delete v;
          }

   }