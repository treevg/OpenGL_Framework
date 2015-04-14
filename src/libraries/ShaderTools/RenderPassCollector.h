#ifndef RENDER_PASS_COLLECTOR_H
#define RENDER_PASS_COLLECTOR_H

#include <vector>
#include "ShaderProgram.h"
#include "FrameBufferObject.h"
#include "VertexArrayObject.h"
#include "RenderPassBase.h"
#include "RenderPassModel.h"

using namespace std;

class RenderPassCollector
{
public:
	RenderPassCollector(vector<VertexArrayObject*> objects, ShaderProgram* shaderProgram);
	

	RenderPassCollector(vector<VertexArrayObject*> objects,  ShaderProgram* shaderProgram, int width, int height);
	RenderPassCollector(vector<VertexArrayObject*> objects,  ShaderProgram* shaderProgram, FrameBufferObject* frameBufferObject);
	
	void run();

    void setShaderProgramm(ShaderProgram*);
    void setFrameBufferObject(FrameBufferObject*);
    ShaderProgram* getShaderProgramm() const;
    FrameBufferObject* getFrameBufferObject() const;

    RenderPassCollector* clear(float r, float g, float b, float a);
    GLuint get(std::string name);
    void autoGenerateFrameBufferObject(int width, int height);
    GLuint  getFrameBufferHandle();


    	template <class T>
	RenderPassCollector* update(std::string name, T value) {
	      shaderProgram->update(name, value);
	return this;
	}



    vector<VertexArrayObject*> objects;
   // vector<RenderPassModel*> models;

 	ShaderProgram* shaderProgram;
	FrameBufferObject* frameBufferObject;

      

};

#endif // RENDER_PASS_COLLECTOR_H
