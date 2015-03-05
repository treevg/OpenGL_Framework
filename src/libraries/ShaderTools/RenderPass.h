/**
 * @file   		RenderPass.h
 * @author 		Gerrit Lochmann
 * @date   		@todo
 * @copyright	@todo
 *
 * @brief  		Manages a render pass
 *
 * The RenderPass class manages a single render pass. 
 */

#ifndef RENDER_PASS_H
#define RENDER_PASS_H

#include "VertexArrayObject.h"
#include "FrameBufferObject.h"

class RenderPass
{
	public:
		RenderPass(VertexArrayObject* vertexArrayObject, ShaderProgram* shaderProgram);
		RenderPass(VertexArrayObject* vertexArrayObject, ShaderProgram* shaderProgram, int width, int height);
		RenderPass(VertexArrayObject* vertexArrayObject, ShaderProgram* shaderProgram, FrameBufferObject* frameBufferObject);

		/**
		 * @brief Executes the whole render pass
		 * @return The RenderPass instance
		 */
		RenderPass* run();

		/**
		 * @brief [brief description]
		 * @details [long description]
		 * 
		 * @param width [description]
		 * @param height [description]
		 */
		void autoGenerateFrameBufferObject(int width, int height);

		GLuint get(std::string name);
		ShaderProgram* getShaderProgram();
		VertexArrayObject* getVertexArrayObject();
		FrameBufferObject* getFrameBufferObject();
		RenderPass* setShaderProgram(ShaderProgram* shaderProgram);
		RenderPass* setVertexArrayObject(VertexArrayObject* vertexArrayObject);
		RenderPass* setFrameBufferObject(FrameBufferObject* frameBufferObject);

		/**
		 * @brief Clears the framebuffer and the depth buffer of the render pass
		 *        with a given RGBA color value
		 * 
		 * @param r Red value of the clear color
		 * @param g Green value of the clear color
		 * @param b Blue value of the clear color
		 * @param a Alpha value of the clear color
		 * @return The RenderPass instance
		 */
		RenderPass* clear(float r, float g, float b, float a);
		RenderPass* clear();
		RenderPass* clearDepth();
		RenderPass* texture(std::string name, GLuint textureID);
		RenderPass* texture(std::string name, GLuint textureID, GLuint samplerID);

		template <class T>
		RenderPass* update(std::string name, T value) {
			shaderProgram->update(name, value);
			return this;
		}

	// private:
		// Shader program to use within a render pass
		ShaderProgram* shaderProgram;
		// Vertex array object to render within a render pass 
		VertexArrayObject* vertexArrayObject;
		// Framebuffer object to render to within a render pass
		FrameBufferObject* frameBufferObject;
};

#endif // RENDER_PASS_H
