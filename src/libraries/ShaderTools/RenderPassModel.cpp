#include "RenderPassModel.h"

using namespace std;


RenderPassModel::RenderPassModel(std::vector<Mesh*> meshes, ShaderProgram* shaderProgram)
	    : meshes(meshes), RenderPassBase(shaderProgram){}

RenderPassModel::RenderPassModel(std::vector<Mesh*> meshes, ShaderProgram* shaderProgram, int width, int height)
      : meshes(meshes), RenderPassBase(shaderProgram, width, height){}
	    
	
void RenderPassModel::runOneMesh()  const{
	cout << "drawing one mesh" << endl;
	frameBufferObject->bind();
	shaderProgram->use();
	mesh->draw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPassModel::run(){
	
      frameBufferObject->bind();
    //   cout << "DEBUG: drawing meshes  "<< endl;

	for (int i=0; i<this->meshes.size(); i++ ){
	
        shaderProgram->use();

     if (this->meshes[i]->getTextures().size() > 0) {

      for (MeshTexture tex: this->meshes[i]->getTextures()){
     
    switch( tex.type  ) 
  { 


  	case 'a': 

  	shaderProgram->texture("ambient_text", tex.id) ; 

  	case 'd': 

  	shaderProgram->texture("diffuse_text", tex.id) ; 


    case 's': 

   // shaderProgram->texture("specular_text", tex.id)
    ; 

      case 'e': 

  //  shaderProgram->texture("emission_text", tex.id) 
      ; 

}
    	}

    } 
        
  	vertexArrayObject->draw();       
        
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	
}