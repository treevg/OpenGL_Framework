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
	ShaderProgram(std::vector<std::string> attachShaders);
	void use();
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
	void printUniformInfo();
	void printInputInfo();
	void printOutputInfo();

private:
	GLuint shaderProgramHandle;

	struct Info{
		GLenum type;
		GLuint location;
		std::string out() {
			return std::to_string(location) + " haha";
		}
	};
	std::map<std::string, Info> uniformMap;
	std::map<std::string, Info> inputMap;
	std::map<std::string, Info> outputMap;

	bool hasEnding (std::string const &fullString, std::string const &ending);
	void attachShader(std::string filename);
	void attachShader(GLenum shaderType, std::string filename);
	std::string loadShaderSource(std::string filename);
	void printShaderProgramInfoLog();
	void mapShaderProperties(GLenum interface, std::map<std::string, Info>* map);
	void printInfo(std::map<std::string, Info>* map);
	std::string getTypeString(GLenum type);
};

#endif // SHADER_PROGRAM_H