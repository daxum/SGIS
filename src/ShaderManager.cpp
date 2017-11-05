/******************************************************************************
 * SGIS - A simple game involving squares
 * Copyright (C) 2017
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#include <sstream>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "ShaderManager.hpp"
#include "Logger.hpp"

using namespace std;

ShaderManager::~ShaderManager() {
	for (auto s : shaders) {
		delete s.second;
	}

	shaders.clear();
}

const Shader* ShaderManager::getShader(string name) const {
	return shaders.at(name);
}

void ShaderManager::loadShader(std::string name, std::string vertName, std::string fragName) {
	if (shaders.count(name) == 0) {
		shaders.insert(make_pair(name, new Shader(createProgram("shaders/" + vertName + ".vert", "shaders/" + fragName + ".frag"))));
	}
	else {
		Logger::warn() << "Attempted to load duplicate shader with name \"" << name << "\"\n";
	}
}

GLuint ShaderManager::createProgram(string vertexName, string fragmentName) {
	//Create shaders and program

	GLuint vertexShader = createShader(vertexName, GL_VERTEX_SHADER);
	GLuint fragmentShader = createShader(fragmentName, GL_FRAGMENT_SHADER);
	GLuint shaderProgram = glCreateProgram();

	//If something went wrong, abort

	if (vertexShader == 0 || fragmentShader == 0 || shaderProgram == 0) {
		glDeleteProgram(shaderProgram);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		Logger::fatal() << "Program loading failed" << "\n";
		throw runtime_error("Program loading failed");
	}

	//Link program

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	int linked = 0;

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked);

	if (linked == 0) {
		Logger::fatal() << "Program linking failed\n";
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		Logger::fatal() << "------------ Program Link Log ------------\n";
		Logger::fatal() << infoLog << "\n";
		Logger::fatal() << "---------------- End Log -----------------\n";

		glDeleteProgram(shaderProgram);

		throw runtime_error("Linking failed for program using \"" + vertexName + "\" and \"" + fragmentName + "\"");
	}

	Logger::info() << "Successfully built program from \"" + vertexName + "\" and \"" + fragmentName + "\"\n";

	return shaderProgram;
}

GLuint ShaderManager::createShader(string filename, GLenum type) {
	//Load source and create shader

	string source = loadShaderSource(filename);

	GLuint shader = glCreateShader(type);

	if (shader == 0) {
		Logger::fatal() << "Could not allocate shader" << "\n";
		throw runtime_error("Could not allocate shader");
	}

	//Set source and compile shader

	const char* cSource = source.c_str();


	glShaderSource(shader, 1, &cSource, nullptr);

	glCompileShader(shader);

	//Log if compilation failed

	GLint status = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (!status) {
		Logger::fatal() << "Failed to compile shader \"" << filename << "\"\n";
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);

		Logger::fatal() << "--------- Shader Compilation Log ---------\n";
		Logger::fatal() << infoLog << "\n";
		Logger::fatal() << "---------------- End Log -----------------\n";

		glDeleteShader(shader);
		throw runtime_error("Failed to compile shader \"" + filename + "\"");
	}

	Logger::info() << "Loaded and compiled shader from \"" << filename << "\"\n";

	return shader;
}

string ShaderManager::loadShaderSource(string fileName) {
	ifstream inFile;
	ostringstream sourceStream;

	inFile.exceptions(ifstream::failbit | ifstream::badbit);

	try {
		inFile.open(fileName);
		sourceStream << inFile.rdbuf();
		inFile.close();
	}
	catch(const ifstream::failure& e) {
		Logger::fatal() << "Couldn't read shader source from file \"" << fileName << "\": " << e.what() << "\n";
		throw runtime_error("Couldn't read shader source");
	}

	return sourceStream.str();
}

