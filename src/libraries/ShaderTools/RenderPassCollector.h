#ifndef RENDER_PASS_COLLECTOR_H
#define RENDER_PASS_COLLECTOR_H

#include <vector>
#include <map>
#include <string>
#include "ShaderProgram.h"
#include "FrameBufferObject.h"
#include "VertexArrayObject.h"
#include "RenderPassBase.h"
#include "RenderPassModel.h"
#include "ShaderProgram.h"

using namespace std;

class RenderPassCollector
{
public:
	RenderPassCollector(vector<VertexArrayObject*> objects,  map<string, ShaderProgram*> shaderProgramms);
	RenderPassCollector(vector<VertexArrayObject*> objects,  map<string, ShaderProgram*> shaderProgramms, int width, int height);
	RenderPassCollector(vector<VertexArrayObject*> objects,  ShaderProgram* shaderProgram, FrameBufferObject* frameBufferObject);
	
	void run();

    void setShaderProgramm(ShaderProgram*);
    RenderPassCollector* setFrameBufferObject(FrameBufferObject*);
    ShaderProgram* getShaderProgramm() const;
    FrameBufferObject* getFrameBufferObject() const;

    RenderPassCollector* clear(float r, float g, float b, float a);
    GLuint get(std::string name);
    void autoGenerateFrameBufferObject(int width, int height);
    GLuint  getFrameBufferHandle();
    RenderPassCollector*  addVAOS(vector<VertexArrayObject*> moreObjects);
    RenderPassCollector*  addVertexArrayObject( VertexArrayObject* obj);
    RenderPassCollector*  renderSkybox( mat4 view);


    	template <class T>
	RenderPassCollector* update(std::string name, T value, string key) {
	      shaderProgramms[key]->update(name, value);
	return this;
	}



    vector<VertexArrayObject*> objects;
    std::map<string, ShaderProgram*> shaderProgramms;

 	
	FrameBufferObject* frameBufferObject;

      

};

#endif // RENDER_PASS_COLLECTOR_H
