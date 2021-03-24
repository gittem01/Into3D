#pragma once

#include <glad/glad.h> // include glad to get the required OpenGL headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

class Shader
{
public:
	static std::vector<std::string> precompiledShaders;
	static std::vector<unsigned int> precompiledShaderIDs;

	// the program ID
	unsigned int ID;

	std::string shaderNames[3] = {"vertexShader.vert", "geometryShader.geom", "fragmentShader.frag"};
	unsigned int shaderTypes[3] = { GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER };
	unsigned int* shaders = (unsigned int*)malloc(sizeof(unsigned int) * 3);
	
	// constructor reads and builds the shader
	Shader(std::string folderName);
	static std::string readFile(const char* path);
	static unsigned int createShader(unsigned int shaderType, std::string str);
	void createProgram();

	// use/activate the shader
	void use();

	// utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const char* name, glm::mat4 value) const;
	void setVec2(const char* name, float x, float y) const;
	void setVec3(const char* name, float x, float y, float z) const;
	void setVec4(const char* name, float x, float y, float z, float t) const;
};
