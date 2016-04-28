/**
 * @file   		ShaderProgram.h
 * @author 		Gerrit Lochmann
 * @date   		@todo
 * @copyright	@todo
 *
 * @brief  		OpenGL shader program wrapper
 *
 * The ShaderProgram class wraps OpenGL Shader Programs and maintains uniform
 * as input/output-buffers using standard c++ maps.
 */
#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <GL/glew.h>

#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <fstream>
#include <glm/ext.hpp>
#include <map>
#include "ShaderTools/ShaderStorageBufferObject.h"


/**
 * @brief 	Verwaltet ein Shader Programm
 */
class ShaderProgram {
public:

	/**
	 * @brief Creates an empty shader program
	 */
	ShaderProgram();

	/**
	 * @brief Creates a shader program from a given list of shader code files
	 *
	 * @param attachShaders List of GLSL source code files
	 */
	// ShaderProgram(std::vector<std::string> attachShaders);
	//MSVC Workaround
	ShaderProgram(std::string a, std::string b);
	ShaderProgram(std::string a, std::string b, std::string c);
	ShaderProgram(std::string a, std::string b, std::string c, std::string d);
	ShaderProgram(std::string a, std::string b, std::string c, std::string d, std::string e);

	/**
	 * @brief Creates a shader program from a given shader code file
	 *
	 * @param type Type of the shader
	 * @param path Filepath to the shader source code file
	 */
	ShaderProgram(GLenum type, std::string path);

	/**
	 * @brief Activates the shader
	 * @details After this function call, the GPU will render everything using
	 *          this shader
	 */
	void use();

	virtual ShaderProgram* texture(std::string name, GLuint textureHandle);
	virtual ShaderProgram* texture(std::string name, GLuint textureHandle, GLuint samplerHandle);

	/**
	 * @brief Updates a boolean uniform variable
	 *
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The value to update the unform with
	 *
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, bool value);
	/**
	 * @brief Updates an integer uniform variable
	 *
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The value to update the unform with
	 *
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, int value);
	/**
	 * @brief Updates a float uniform variable
	 *
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The value to update the unform with
	 *
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, float value);
	/**
	 * @brief Updates a double uniform variable
	 *
	 * @param name  Name of the uniform variable in GLSL
	 * @param value The value to update the unform with
	 *
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, double value);
	/**
	 * @brief Updates a 2D integer vector uniform variable
	 *
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The value to update the unform with
	 *
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, glm::ivec2 vector);
	/**
	 * @brief Updates a 3D integer vector uniform variable
	 *
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The value to update the unform with
	 *
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, glm::ivec3 vector);
	/**
	 * @brief Updates a 4D integer vector uniform variable
	 *
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The value to update the unform with
	 *
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, glm::ivec4 vector);
	/**
	 * @brief Updates a 2D float vector uniform variable
	 *
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The value to update the unform with
	 *
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, glm::vec2 vector);
	/**
	 * @brief Updates a 3D float vector uniform variable
	 *
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The value to update the unform with
	 *
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, glm::vec3 vector);
	/**
	 * @brief Updates a 4D float vector uniform variable
	 *
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The value to update the unform with
	 *
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, glm::vec4 vector);
	/**
	 * @brief Updates a 2x2 matrix uniform variable
	 *
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The value to update the unform with
	 *
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, glm::mat2 matrix);
	/**
	 * @brief Updates a 3x3 matrix uniform variable
	 *
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The value to update the unform with
	 *
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, glm::mat3 matrix);
	/**
	 * @brief Updates a 4x4 matrix uniform variable
	 *
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The value to update the unform with
	 *
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, glm::mat4 matrix);
	/**
	 * @brief Updates a list of 2D vector uniform variables
	 *
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The vector witch stores the values to update the unform with
	 *
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, std::vector<glm::vec2> vector);
	/**
	 * @brief Updates a list of 3D vector uniform variables
	 *
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The vector witch stores the values to update the unform with
	 *
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, std::vector<glm::vec3> vector);
	/**
	 * @brief Updates a list of 4D vector uniform variables
	 *
	 * @param name 	Name of the uniform variable in GLSL
	 * @param value The vector witch stores the values to update the unform with
	 *
	 * @return The shader program
	 */
	ShaderProgram* update(std::string name, std::vector<glm::vec4> vector);
	/**
 	* @brief Updates the binding of a Shader Storage Block
 	* @param name Name of the Shader Storage Block
 	* @param ssbo The Shader Storage Buffer Object to be bound
 	* @return
 	*/
	ShaderProgram* update(std::string name, ShaderStorageBufferObject *ssbo);

	/**
	 * @brief Logs all active bound uniforms to the console
	 */
	void printUniformInfo();
	/**
	 * @brief Logs all active bound input buffers to the console
	 */
	void printInputInfo();
	/**
	 * @brief Logs all active bound output buffers to the console
	 */
	void printOutputInfo();
	/**
	 * @brief Returns the shader program handle
	 * @return Handle of the shader program
	 */
	GLuint getProgramHandle();

	/**
	 * @brief Struct for possible GLSL bindings
	 */
	struct Info{
		GLenum type;
		GLuint location;
	};
	/**
	 * @brief Maintains all used and bound uniform locations
	 */
	std::map<std::string, Info> uniformMap;
	/**
	 * @brief Maintains all used input buffers
	 */
	std::map<std::string, Info> inputMap;
	/**
	 * @brief Maintains all used output buffers
	 */
	std::map<std::string, Info> outputMap;

	struct TextureObject{
		GLenum textureHandle;
		GLenum samplerHandle;
		std::string name;
		bool equals(TextureObject o) {
			// return textureHandle == o.textureHandle && samplerHandle == o.textureHandle;
			return name == o.name;
		}
	};
	std::vector<TextureObject> textureList;

protected:
	GLuint shaderProgramHandle;
	bool errorOccured = false;

	/**
	 * @brief Links the shader program against the OpenGL host code
	 */
	void link();
	/**
	 * @brief Looks for uniform variables to update in the corresponding uniform
	 *        map
	 *
	 * @param name Uniform variables name
	 * @param type Uniform variables type
	 *
	 * @return Info pointer
	 */
	Info* checkUpdate(std::string name, std::string type);
	/**
	 * @brief Checks for a shader code filepath to have valid shader code file
	 *        ending
	 *
	 * @param filename Filename of the shader code file
	 * @param typeLine Fileending to compare against
	 *
	 * @return Boolean if the file compares with the given type
	 */
	bool hasValidType(std::string filename, std::string typeLine);
	bool hasEnding (std::string fullString, std::string ending);
	virtual void attachShader(std::string filename);
	void attachShader(GLenum shaderType, std::string filename);
	/**
	 * @brief Loads a shader source code file from disk
	 *
	 * @param filename Filepath to the shader code file
	 * @return A sring containing the shader code
	 */
	std::string loadShaderSource(std::string filename);
	/**
	 * @brief Prints informations of the shader programs compile and link status
	 *        to the console
	 */
	void printShaderProgramInfoLog();
	void mapShaderProperties(GLenum interf, std::map<std::string, Info>* map);
	void printInfo(std::map<std::string, Info>* map);
	std::string getTypeString(GLenum type);
};

#endif // SHADER_PROGRAM_H
