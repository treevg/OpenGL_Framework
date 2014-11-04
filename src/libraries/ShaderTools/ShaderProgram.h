#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <GL/glew.h>

#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <fstream>
#define GLM_FORCE_RADIANS
#include <glm/ext.hpp>
#include <map>

class ShaderProgram {
public:
	ShaderProgram();
	ShaderProgram(std::vector<std::string> attachShaders);
	void use();
	virtual ShaderProgram* texture(std::string name, GLuint textureHandle);
	virtual ShaderProgram* texture(std::string name, GLuint textureHandle, GLuint samplerHandle);
	ShaderProgram* update(std::string name, bool value);
	ShaderProgram* update(std::string name, int value);
	ShaderProgram* update(std::string name, float value);
	ShaderProgram* update(std::string name, double value);
	ShaderProgram* update(std::string name, glm::ivec2 vector);
	ShaderProgram* update(std::string name, glm::ivec3 vector);
	ShaderProgram* update(std::string name, glm::ivec4 vector);
	ShaderProgram* update(std::string name, glm::vec2 vector);
	ShaderProgram* update(std::string name, glm::vec3 vector);
	ShaderProgram* update(std::string name, glm::vec4 vector);
	ShaderProgram* update(std::string name, glm::mat2 matrix);
	ShaderProgram* update(std::string name, glm::mat3 matrix);
	ShaderProgram* update(std::string name, glm::mat4 matrix);
	ShaderProgram* update(std::string name, std::vector<glm::vec2> vector);
	ShaderProgram* update(std::string name, std::vector<glm::vec3> vector);
	ShaderProgram* update(std::string name, std::vector<glm::vec4> vector);
	void printUniformInfo();
	void printInputInfo();
	void printOutputInfo();

	struct Info{
		GLenum type;
		GLuint location;
	};
	std::map<std::string, Info> uniformMap;
	std::map<std::string, Info> inputMap;
	std::map<std::string, Info> outputMap;

protected:
	GLuint shaderProgramHandle;
	int currentTextureUnit;
	bool errorOccured = false;

	void link();
	Info* checkUpdate(std::string name, std::string type);
	bool hasValidType(std::string filename, std::string typeLine);
	bool hasEnding (std::string fullString, std::string ending);
	virtual void attachShader(std::string filename);
	void attachShader(GLenum shaderType, std::string filename);
	std::string loadShaderSource(std::string filename);
	void printShaderProgramInfoLog();
	void mapShaderProperties(GLenum interface, std::map<std::string, Info>* map);
	void printInfo(std::map<std::string, Info>* map);
	std::string getTypeString(GLenum type);
};

#endif // SHADER_PROGRAM_H
