#include "RenderPassModel.h"

using namespace std;


RenderPassModel::RenderPassModel(std::vector<VertexArrayObject*> meshes, ShaderProgram* shaderProgram)
	    : meshes(meshes), RenderPassBase(shaderProgram){}

RenderPassModel::RenderPassModel(std::vector<VertexArrayObject*> meshes, ShaderProgram* shaderProgram, int width, int height)
      : meshes(meshes), RenderPassBase(shaderProgram, width, height){}
	    
	
void RenderPassModel::runOneMesh()  const{
	cout << "drawing one mesh" << endl;
	frameBufferObject->bind();
	shaderProgram->use();
	mesh->draw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void RenderPassModel::run(){

 // cout << " RENDERING MESH " << endl;
	
      frameBufferObject->bind();


	for (int i=0; i<this->meshes.size(); i++ ){
	
        shaderProgram->use();

    if (this->meshes[i]->getTextures().size() > 0) {

      for (MeshTexture tex: this->meshes[i]->getTextures()){
        
     //   cout <<  " ------  DEBUG: TEXTURE ID "  << tex.id << endl; 
     
    switch( tex.type  ) { 

  	case 'a': 

  	shaderProgram->textureModel("ambient_text", tex.id) ; 

  	case 'd': 

  	shaderProgram->textureModel("diffuse_text", tex.id) ; 

    case 's': 

    shaderProgram->textureModel("specular_text", tex.id)
    ; 

      case 'e': 

  //  shaderProgram->textureModel("emission_text", tex.id) 
      ; 

}
    	}

    } 
      


  	 meshes[i]->draw();     
    
        
	}

	
 glBindFramebuffer(GL_FRAMEBUFFER, 0);

	
}

RenderPassModel* RenderPassModel::setFrameBufferObject(FrameBufferObject* frameBufferObject) {
  this->frameBufferObject = frameBufferObject;
  return this;
}
