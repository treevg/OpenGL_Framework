#include "RenderPassModel.h"

using namespace std;


RenderPassModel::RenderPassModel(std::vector<VertexArrayObject*> meshes, ShaderProgram* shaderProgram)
	    : meshes(meshes), RenderPassBase(shaderProgram){}

RenderPassModel::RenderPassModel(std::vector<VertexArrayObject*> meshes, ShaderProgram* shaderProgram, int width, int height)
      : meshes(meshes), RenderPassBase(shaderProgram, width, height){}
	    
	
void RenderPassModel::runOneMesh()  {
	cout << "drawing one mesh" << endl;
	frameBufferObject->bind();
	shaderProgram->use();
	mesh->draw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void RenderPassModel::run(){

	
      frameBufferObject->bind();


	for (int i=0; i<this->meshes.size(); i++ ){
	
        shaderProgram->use();

    if (this->meshes[i]->getTextures().size() > 0) {

      for (MeshTexture tex: this->meshes[i]->getTextures()){
        
     switch( tex.type  ) { 

  	   case 'a': 

         	shaderProgram->textureModel("ambient_text", tex.id) ; 

      	case 'd': 

     	    shaderProgram->textureModel("diffuse_text", tex.id) ; 

        case 's': 

         shaderProgram->textureModel("specular_text", tex.id); 

      

}
    	}

    } 

   
      shaderProgram->update("Ka", meshes[i]->getMaterial().ka);
      shaderProgram->update("Kd", meshes[i]->getMaterial().kd);
      shaderProgram->update("Ks", meshes[i]->getMaterial().ks);
    
  	 meshes[i]->draw();     
    
        
	}

	
 glBindFramebuffer(GL_FRAMEBUFFER, 0);

	
}



RenderPassModel* RenderPassModel::setFrameBufferObject(FrameBufferObject* frameBufferObject) {
  this->frameBufferObject = frameBufferObject;
  return this;
}
