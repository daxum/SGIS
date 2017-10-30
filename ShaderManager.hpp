#pragma once

#include <unordered_map>
#include <string>
#include "combinedGl.h"
#include "Shader.hpp"

class ShaderManager {
public:
	~ShaderManager();

	const Shader* getShader(std::string name) const;

	void loadShader(std::string name, std::string vertName, std::string fragName);

private:
	std::unordered_map<std::string, Shader*> shaders;
	GLuint createProgram(std::string vertexName, std::string fragmentName);
	GLuint createShader(std::string filename, GLenum type);
	std::string loadShaderSource(std::string fileName);

};
