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

#include <stdexcept>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.hpp"
#include "Logger.hpp"

using namespace std;

Shader::Shader(GLuint id) : id(id) {}

Shader::~Shader() {
	glDeleteProgram(id);
}

void Shader::use() const {
	glUseProgram(id);
}

void Shader::setUniformMat4(string name, glm::mat4 matrix) const {
	GLuint uniformLoc = glGetUniformLocation(id, name.c_str());
	glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniformVec2(string name, glm::vec2 vec) const {
	GLuint uniformLoc = glGetUniformLocation(id, name.c_str());
	glUniform2fv(uniformLoc, 1, glm::value_ptr(vec));
}

void Shader::setUniformVec3(string name, glm::vec3 vec) const {
	GLuint uniformLoc = glGetUniformLocation(id, name.c_str());
	glUniform3fv(uniformLoc, 1, glm::value_ptr(vec));
}
