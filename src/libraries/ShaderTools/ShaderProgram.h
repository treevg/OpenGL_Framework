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


/**
 * @brief 	Verwaltet ein Shader Programm
 */
class ShaderProgram {
public:

	/**
	 * @brief Erzeugt ein leeres Shader Programm
	 */
	ShaderProgram();

	/**
	 * @brief Erzeugt ein Shader Programm mit einer gebenen List von Shadern
	 *        plus ihren GLSL-Sourcecodes
	 * 
	 * @param attachShaders Liste von GLSL-Sourcecodes
	 */
	ShaderProgram(std::vector<std::string> attachShaders);

	/**
	 * @brief Erzeugt ein Shader Prorgamm mit einem gegebenen Shader
	 * 
	 * @param type Typ des Shaders
	 * @param path Pfad zum Sourcecode des Shaders
	 */
	ShaderProgram(GLenum type, std::string path);

	/**
	 * @brief Setzt das Shader Programm als aktiv
	 * @details Ab dem Aufruf dieser Funktion wird mit diesem Shader Programm
	 * gerendert
	 */
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
	
	/**
	 * @brief Schreibt einen Log über gebundene Uniforms auf die Konsole
	 */
	void printUniformInfo();
	/**
	 * @brief Schreibt einen Log über gebundene Input Buffer auf die Konsole
	 */
	void printInputInfo();
	/**
	 * @brief Schreibt einen Log über gebundene Output Buffer auf die Konsole
	 */
	void printOutputInfo();
	/**
	 * @brief Gibt eine Referenz auf Shader Programm zurück
	 * @return Shader Programm Referenz
	 */
	GLuint getProgramHandle();

	/**
	 * @brief Struct zur Verwaltung von GLSL-Bindings
	 */
	struct Info{
		GLenum type;
		GLuint location;
	};
	/**
	 * @brief Die Uniform Map speichert alle vom Shader
	 *        Programm gebundenen Uniform Variablen
	 */
	std::map<std::string, Info> uniformMap;
	/**
	 * @brief Die Input Map speichert alle vom Shader Programm
	 *        gebundenen Input Buffer
	 */
	std::map<std::string, Info> inputMap;
	/**
	 * @brief Die Output Map speichert alle vom Shader Programm
	 *        gebundenen Output Buffer
	 */
	std::map<std::string, Info> outputMap;

	struct TextureObject{
		GLenum textureHandle;
		GLenum samplerHandle;
		bool equals(TextureObject o) {
			return textureHandle == o.textureHandle && samplerHandle == o.textureHandle;
		}
	};
	std::vector<TextureObject> textureList;

protected:
	GLuint shaderProgramHandle;
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
