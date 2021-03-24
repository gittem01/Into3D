#include "Shader.h"

std::vector<std::string> Shader::precompiledShaders;
std::vector<unsigned int> Shader::precompiledShaderIDs;

std::string Shader::readFile(const char* path) {
	if (!path) {
		return std::string("");
	}
	std::string shaderCode;
	std::ifstream ShaderFile;
	std::stringstream ShaderStream;
	ShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		ShaderFile.open(path);
		ShaderStream << ShaderFile.rdbuf();
		ShaderFile.close();
		shaderCode = ShaderStream.str();
		return shaderCode;
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
}

unsigned int Shader::createShader(unsigned int shaderType, std::string code) {
	if (code.size() < 2) {
		return NULL;
	}
	const char* shaderCode = code.c_str();
	int success;
	char infoLog[512];
	unsigned int vertex = glCreateShader(shaderType);
	glShaderSource(vertex, 1, &shaderCode, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	};
	return vertex;
}

void Shader::createProgram() {

	this->ID = glCreateProgram();

	for (int i = 0; i < 3; i++) {
		glAttachShader(this->ID, this->shaders[i]);
	}
	int success;
	char infoLog[512];
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
			infoLog << std::endl;
	}
	for (int i = 0; i < 3; i++) {
		glDeleteShader(this->shaders[i]);
	}
}

Shader::Shader(std::string folderName)
{
	for (int i = 0; i < Shader::precompiledShaders.size(); i++) {
		if (Shader::precompiledShaders.at(i).compare(folderName) == 0) {
			this->ID = Shader::precompiledShaderIDs.at(i);
			return;
		}
	}
	for (int i = 0; i < 3; i++) {
		std::string fullPath = folderName + shaderNames[i];
		std::string shaderCode = this->readFile(fullPath.c_str());
		this->shaders[i] = this->createShader(shaderTypes[i], shaderCode);
	}
	this->createProgram();
	
	Shader::precompiledShaders.push_back(folderName);
	Shader::precompiledShaderIDs.push_back(this->ID);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const char* name, glm::mat4 value) const {
	unsigned int loc = glGetUniformLocation(this->ID, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec2(const char* name, float x, float y) const {
	unsigned int loc = glGetUniformLocation(this->ID, name);
	glUniform2f(loc, x, y);
}

void Shader::setVec3(const char* name, float x, float y, float z) const {
	unsigned int loc = glGetUniformLocation(this->ID, name);
	glUniform3f(loc, x, y, z);
}

void Shader::setVec4(const char* name, float x, float y, float z, float t) const {
	unsigned int loc = glGetUniformLocation(this->ID, name);
	glUniform4f(loc, x, y, z, t);
}
