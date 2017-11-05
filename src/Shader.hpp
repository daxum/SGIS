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

#include <string>
#include <glm/glm.hpp>
#include "combinedGl.h"

class Shader {
	friend class ShaderManager;
public:
	const GLuint id;

	Shader() = delete;
	~Shader();

	void use() const;

	//Insert uniform stuff here as needed
	void setUniformMat4(std::string name, glm::mat4 matrix) const;
	void setUniformVec2(std::string name, glm::vec2 vec) const;
	void setUniformVec3(std::string name, glm::vec3 vec) const;

private:
	Shader(GLuint id);
};

