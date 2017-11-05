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
