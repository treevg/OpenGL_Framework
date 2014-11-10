#include "ShaderProgram.h"
#include <algorithm>

using namespace std;
using namespace glm;

ShaderProgram::ShaderProgram(){
}

ShaderProgram::ShaderProgram(vector<string> attachShaders) {
    shaderProgramHandle = glCreateProgram();

    for (string s : attachShaders) {
    	attachShader(SHADERS_PATH + s);
    }

    link();

    mapShaderProperties(GL_UNIFORM, &uniformMap);
    mapShaderProperties(GL_PROGRAM_INPUT, &inputMap);
    mapShaderProperties(GL_PROGRAM_OUTPUT, &outputMap);
}

ShaderProgram::ShaderProgram(GLenum type, string path){
	shaderProgramHandle = glCreateProgram();

	attachShader(type, SHADERS_PATH+path);
	link();

	mapShaderProperties(GL_UNIFORM, &uniformMap);
	mapShaderProperties(GL_PROGRAM_INPUT, &inputMap);
	mapShaderProperties(GL_PROGRAM_OUTPUT, &outputMap);
}

void ShaderProgram::use() {
	currentTextureUnit = 0;
	glUseProgram(shaderProgramHandle);
}

ShaderProgram* ShaderProgram::texture(std::string name, GLuint textureHandle) {
	Info* updateInfo = checkUpdate(name, "sampler2D");
	if (updateInfo != NULL) {	
		glUseProgram(shaderProgramHandle);
		glUniform1i(updateInfo->location, currentTextureUnit);
		glActiveTexture(GL_TEXTURE0 + currentTextureUnit);
	    glBindTexture(GL_TEXTURE_2D, textureHandle);
		currentTextureUnit++;
	}
	return this;
}

ShaderProgram* ShaderProgram::texture(std::string name, GLuint textureHandle, GLuint samplerHandle) {
	Info* updateInfo = checkUpdate(name, "sampler2D");
	if (updateInfo != NULL) {	
		glUseProgram(shaderProgramHandle);
		glUniform1i(updateInfo->location, currentTextureUnit);
		glActiveTexture(GL_TEXTURE0 + currentTextureUnit);
	    glBindTexture(GL_TEXTURE_2D, textureHandle);
	    glBindSampler(currentTextureUnit, samplerHandle);
		currentTextureUnit++;
	}
	return this;
}

ShaderProgram* ShaderProgram::update(string name, bool value) {
	Info* updateInfo = checkUpdate(name, "bool");
	if (updateInfo != NULL) {	
		glUseProgram(shaderProgramHandle);
		glUniform1i(updateInfo->location, value);
	}
	return this;
}

ShaderProgram* ShaderProgram::update(string name, int value) {
	Info* updateInfo = checkUpdate(name, "int");
	if (updateInfo != NULL) {	
		glUseProgram(shaderProgramHandle);
		glUniform1i(updateInfo->location, value);
	}
	return this;
}

ShaderProgram* ShaderProgram::update(string name, float value) {
	Info* updateInfo = checkUpdate(name, "float");
	if (updateInfo != NULL) {	
		glUseProgram(shaderProgramHandle);
		glUniform1f(updateInfo->location, value);
	}
	return this;
}

ShaderProgram* ShaderProgram::update(string name, double value) {
	Info* updateInfo = checkUpdate(name, "double");
	if (updateInfo != NULL) {	
		glUseProgram(shaderProgramHandle);
		glUniform1f(updateInfo->location, value);
	}
	return this;
}

ShaderProgram* ShaderProgram::update(string name, ivec2 vector) {
	Info* updateInfo = checkUpdate(name, "ivec2");
	if (updateInfo != NULL) {	
		glUseProgram(shaderProgramHandle);
		glUniform2iv(updateInfo->location, 1, glm::value_ptr(vector));
	}
	return this;
}

ShaderProgram* ShaderProgram::update(string name, ivec3 vector) {
	Info* updateInfo = checkUpdate(name, "ivec3");
	if (updateInfo != NULL) {	
		glUseProgram(shaderProgramHandle);
		glUniform3iv(updateInfo->location, 1, glm::value_ptr(vector));
	}
	return this;
}

ShaderProgram* ShaderProgram::update(string name, ivec4 vector) {
	Info* updateInfo = checkUpdate(name, "ivec4");
	if (updateInfo != NULL) {	
		glUseProgram(shaderProgramHandle);
		glUniform4iv(updateInfo->location, 1, glm::value_ptr(vector));
	}
	return this;
}

ShaderProgram* ShaderProgram::update(string name, vec2 vector) {
	Info* updateInfo = checkUpdate(name, "vec2");
	if (updateInfo != NULL) {	
		glUseProgram(shaderProgramHandle);
		glUniform2fv(updateInfo->location, 1, glm::value_ptr(vector));
	}
	return this;
}

ShaderProgram* ShaderProgram::update(string name, vec3 vector) {
	Info* updateInfo = checkUpdate(name, "vec3");
	if (updateInfo != NULL) {	
		glUseProgram(shaderProgramHandle);
		glUniform3fv(updateInfo->location, 1, glm::value_ptr(vector));
	}
	return this;
}

ShaderProgram* ShaderProgram::update(string name, vec4 vector) {
	Info* updateInfo = checkUpdate(name, "vec4");
	if (updateInfo != NULL) {	
		glUseProgram(shaderProgramHandle);
		glUniform4fv(updateInfo->location, 1, glm::value_ptr(vector));
	}
	return this;
}

ShaderProgram* ShaderProgram::update(string name, mat2 matrix) {
	Info* updateInfo = checkUpdate(name, "mat2");
	if (updateInfo != NULL) {	
		glUseProgram(shaderProgramHandle);
		glUniformMatrix2fv(updateInfo->location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	return this;
}

ShaderProgram* ShaderProgram::update(string name, mat3 matrix) {
	Info* updateInfo = checkUpdate(name, "mat3");
	if (updateInfo != NULL) {	
		glUseProgram(shaderProgramHandle);
		glUniformMatrix3fv(updateInfo->location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	return this;
}

ShaderProgram* ShaderProgram::update(string name, mat4 matrix) {
	Info* updateInfo = checkUpdate(name, "mat4");
	if (updateInfo != NULL) {	
		glUseProgram(shaderProgramHandle);
		glUniformMatrix4fv(updateInfo->location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	return this;
}

ShaderProgram* ShaderProgram::update(string name, std::vector<glm::vec2> vector) {
	Info* updateInfo = checkUpdate(name, "vec2");
	if (updateInfo != NULL) {
		glUseProgram(shaderProgramHandle);
		glUniform2fv(updateInfo->location, sizeof(vector), glm::value_ptr((&vector[0])[0]));
	}
	return this;
}

ShaderProgram* ShaderProgram::update(string name, std::vector<glm::vec3> vector) {
	Info* updateInfo = checkUpdate(name, "vec3");
	if (updateInfo != NULL) {
		glUseProgram(shaderProgramHandle);
		glUniform3fv(updateInfo->location, sizeof(vector), glm::value_ptr((&vector[0])[0]));
	}
	return this;
}

ShaderProgram* ShaderProgram::update(string name, std::vector<glm::vec4> vector) {
	Info* updateInfo = checkUpdate(name, "vec4");
	if (updateInfo != NULL) {
		glUseProgram(shaderProgramHandle);
		glUniform4fv(updateInfo->location, sizeof(vector), glm::value_ptr((&vector[0])[0]));
	}
	return this;
}

ShaderProgram::Info* ShaderProgram::checkUpdate(std::string name, std::string type) {
	auto it = uniformMap.find(name);
	if (errorOccured) {
		return NULL;
	}
	if (it == uniformMap.end()) {
		std::cerr << "INVALID UNIFORM UPDATE IN SHADER PROGRAM " << shaderProgramHandle << std::endl
		<< "\"uniform " << type << " " << name << "\" is not set or has been removed by the GLSL compiler" << std::endl << std::endl;
		return NULL;
	}
	return &(it->second);
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

bool ShaderProgram::hasValidType(string filename, string typeLine) {
	std::string delimiter = " ";
	char invalidChars[] = "*.";
	
	for (unsigned int i = 0; i < strlen(invalidChars); ++i) {
	  typeLine.erase (std::remove(typeLine.begin(), typeLine.end(), invalidChars[i]), typeLine.end());
	}
	size_t pos = 0;
	std::string token;
	while ((pos = typeLine.find(delimiter)) != std::string::npos) {
	    token = typeLine.substr(0, pos);
	    if (token.length() > 0 && 0 == filename.compare (filename.length() - token.length(), token.length(), token))
	    	return true;
	    typeLine.erase(0, pos + delimiter.length());
	}
    if (token.length() > 0 && 0 == filename.compare (filename.length() - typeLine.length(), typeLine.length(), typeLine))
		return true;
	return false;
}

bool ShaderProgram::hasEnding (string fullString, string ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

void ShaderProgram::attachShader(string filename) {
	if (hasValidType(filename, ".vert .vs")) {
		attachShader(GL_VERTEX_SHADER, filename);
		return;
	}
	if (hasValidType(filename, ".frag .fs")) {
		attachShader(GL_FRAGMENT_SHADER, filename);
		return;
	}
	if (hasValidType(filename, ".geom .geo")) {
		attachShader(GL_GEOMETRY_SHADER, filename);
		return;
	}
	cerr << "ERROR IN SHADER PROGRAM " << shaderProgramHandle << std::endl
    	<< filename << " filetype invalid" << endl;
    errorOccured = true;
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
        cerr << "ERROR IN SHADER PROGRAM " << shaderProgramHandle << std::endl
        << "Unable to compile " << filename << endl;
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
        cerr << "ERROR IN SHADER PROGRAM " << shaderProgramHandle << std::endl
        << "Unable to read shader source code from " << filename << endl;
    }

    return shaderSrc;
}

void ShaderProgram::link() {
	glLinkProgram(shaderProgramHandle);
	GLint linkStatus;
	glGetProgramiv(shaderProgramHandle, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == GL_FALSE) {
		errorOccured = true;
		printShaderProgramInfoLog();
	}
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
	return "unknown";
	//std::to_string(type);
}


