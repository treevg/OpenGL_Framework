#include "ShaderProgram.h"

using namespace std;
using namespace glm;

ShaderProgram::ShaderProgram(vector<string> attachShaders) {
    shaderProgramHandle = glCreateProgram();

    for (string s : attachShaders) {
    	ShaderProgram::attachShader(SHADERS_PATH + s);
    }

    glLinkProgram(shaderProgramHandle);

    mapShaderProperties(GL_UNIFORM, &uniformMap);
    mapShaderProperties(GL_PROGRAM_INPUT, &inputMap);
    mapShaderProperties(GL_PROGRAM_OUTPUT, &outputMap);
}

void ShaderProgram::use() {
	glUseProgram(shaderProgramHandle);
}

ShaderProgram* ShaderProgram::update(string name, bool value) {
	glUseProgram(shaderProgramHandle);
	glUniform1i(uniformMap.at(name).location, value);
	return this;
}
ShaderProgram* ShaderProgram::update(string name, int value) {
	glUseProgram(shaderProgramHandle);
	glUniform1i(uniformMap.at(name).location, value);
	return this;
}
ShaderProgram* ShaderProgram::update(string name, float value) {
	glUseProgram(shaderProgramHandle);
	glUniform1f(uniformMap.at(name).location, value);
	return this;
}
ShaderProgram* ShaderProgram::update(string name, double value) {
	glUseProgram(shaderProgramHandle);
	glUniform1f(uniformMap.at(name).location, value);
	return this;
}
ShaderProgram* ShaderProgram::update(string name, ivec2 vector) {
	glUseProgram(shaderProgramHandle);
	glUniform2iv(uniformMap.at(name).location, 1, glm::value_ptr(vector));
	return this;
}
ShaderProgram* ShaderProgram::update(string name, ivec3 vector) {
	glUseProgram(shaderProgramHandle);
	glUniform3iv(uniformMap.at(name).location, 1, glm::value_ptr(vector));
	return this;
}
ShaderProgram* ShaderProgram::update(string name, ivec4 vector) {
	glUseProgram(shaderProgramHandle);
	glUniform4iv(uniformMap.at(name).location, 1, glm::value_ptr(vector));
	return this;
}
ShaderProgram* ShaderProgram::update(string name, vec2 vector) {
	glUseProgram(shaderProgramHandle);
	glUniform2fv(uniformMap.at(name).location, 1, glm::value_ptr(vector));
	return this;
}
ShaderProgram* ShaderProgram::update(string name, vec3 vector) {
	glUseProgram(shaderProgramHandle);
	glUniform3fv(uniformMap.at(name).location, 1, glm::value_ptr(vector));
	return this;
}
ShaderProgram* ShaderProgram::update(string name, vec4 vector) {
	glUseProgram(shaderProgramHandle);
	glUniform4fv(uniformMap.at(name).location, 1, glm::value_ptr(vector));
	return this;
}
ShaderProgram* ShaderProgram::update(string name, mat2 matrix) {
	glUseProgram(shaderProgramHandle);
	glUniformMatrix2fv(uniformMap.at(name).location, 1, GL_FALSE, glm::value_ptr(matrix));
	return this;
}
ShaderProgram* ShaderProgram::update(string name, mat3 matrix) {
	glUseProgram(shaderProgramHandle);
	glUniformMatrix3fv(uniformMap.at(name).location, 1, GL_FALSE, glm::value_ptr(matrix));
	return this;
}
ShaderProgram* ShaderProgram::update(string name, mat4 matrix) {
	glUseProgram(shaderProgramHandle);
	glUniformMatrix4fv(uniformMap.at(name).location, 1, GL_FALSE, glm::value_ptr(matrix));
	return this;
}

void ShaderProgram::printUniformInfo() {
	cout << "SHADER PROGRAM " << shaderProgramHandle << " UNIFORM INFO" << endl << endl;
	printInfo(&uniformMap);
}

void ShaderProgram::printInputInfo() {
	cout << "SHADER PROGRAM " << shaderProgramHandle << " INPUT INFO" << endl << endl;
	printInfo(&inputMap);
}

void ShaderProgram::printOutputInfo() {
	cout << "SHADER PROGRAM " << shaderProgramHandle << " OUTPUT INFO" << endl << endl;
	printInfo(&outputMap);
}

bool ShaderProgram::hasEnding (string const &fullString, string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

void ShaderProgram::attachShader(string filename) {
	if (hasEnding(filename, ".vert")) {
		attachShader(GL_VERTEX_SHADER, filename);
	} else if (hasEnding(filename, ".frag")) {
		attachShader(GL_FRAGMENT_SHADER, filename);
	} else if (hasEnding(filename, ".geom")) {
		attachShader(GL_GEOMETRY_SHADER, filename);
	}
}

void ShaderProgram::attachShader(GLenum shaderType, string filename) {
	GLuint shader = glCreateShader(shaderType);

    string code = loadShaderSource(filename);
    const char *codePointer = code.c_str();
    glShaderSource(shader, 1, &codePointer, NULL);
    glCompileShader(shader);

    GLint rvalue;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &rvalue);
    if (!rvalue) {
        cerr << "ERROR IN SHADER PROGRAM " << shaderProgramHandle << ": Unable to compile ";
        cerr << filename << endl;
        exit(30);
    }
    glAttachShader(shaderProgramHandle, shader);
}


string ShaderProgram::loadShaderSource(string filename) {
    string shaderSrc;
    string line;

    ifstream fileIn(filename.c_str());
    if(fileIn.is_open()){
        while(!fileIn.eof()){
            getline(fileIn, line);
            line += "\n";
            shaderSrc += line;
        }
        fileIn.close();
    }
    else
    {
        cerr<< "ERROR IN SHADER PROGRAM " << shaderProgramHandle << ": Unable to read shader source code from " << filename << endl;
    }

    return shaderSrc;
}

void ShaderProgram::printShaderProgramInfoLog() {
    GLint logLength;
    glGetProgramiv(shaderProgramHandle, GL_INFO_LOG_LENGTH, &logLength);
    if(logLength > 0){
        char* log = (char*) malloc(logLength);
        GLsizei written;
        glGetProgramInfoLog(shaderProgramHandle, logLength, &written, log);
        cout << "SHADER PROGRAM " << shaderProgramHandle << " LOG" << endl;
        cout << log << endl;
        free(log);
    }
}

void ShaderProgram::mapShaderProperties(GLenum interface, std::map<std::string, Info>* map) {
	GLint numAttrib = 0;
	glGetProgramInterfaceiv(shaderProgramHandle, interface, GL_ACTIVE_RESOURCES, &numAttrib);


	std::vector<GLenum> properties;
	properties.push_back(GL_NAME_LENGTH);
	properties.push_back(GL_TYPE);
	properties.push_back(GL_ARRAY_SIZE);
	properties.push_back(GL_LOCATION);
	std::vector<GLint> values(properties.size());
	for(int attrib = 0; attrib < numAttrib; ++attrib)
	{
		glGetProgramResourceiv(shaderProgramHandle, interface, attrib, properties.size(),
		&properties[0], values.size(), NULL, &values[0]);
		
		Info info;
		info.type = values[1];
		info.location = values[3];

		std::vector<GLchar> nameData(256);
		nameData.resize(properties[0]); //The length of the name.
		glGetProgramResourceName(shaderProgramHandle, interface, attrib, nameData.size(), NULL, &nameData[0]);
		std::string name = std::string((char*)&nameData[0], nameData.size() - 1);
		name = std::string(name.c_str());
		(*map)[name] = info;
	}
}

void ShaderProgram::printInfo(std::map<std::string, Info>* map) {
	for(auto e : *map) {
		cout <<
		"name: " << e.first << endl <<
		"type: " << getTypeString(e.second.type) << endl <<
		"location: " << e.second.location << endl << endl;
	}
}


string ShaderProgram::getTypeString(GLenum type) {
	switch (type) {
	case 35670: 
		return "bool";
	case 5124: 
		return "int";
	case 5126: 
		return "float";
	case 35667: 
		return "ivec2";
	case 35668: 
		return "ivec3";
	case 35669: 
		return "ivec4";
	case 35664: 
		return "vec2";
	case 35665: 
		return "vec3";
	case 35666: 
		return "vec4";
	case 35674: 
		return "mat2";
	case 35675: 
		return "mat3";
	case 35676: 
		return "mat4";
	case 35677: 
		return "sampler1D";
	case 35678: 
		return "sampler2D";
	case 35679: 
		return "sampler3D";
	}
	return std::to_string(type);
}


