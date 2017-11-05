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

#include <vector>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include "combinedGl.h"
#include "Shader.hpp"

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

class Mesh {
	friend class Model;
public:
	~Mesh();

	void draw(const Shader* shader) const;

private:
	GLuint vertexArray;
	GLuint vertexBuffer;
	GLuint elementBuffer;

	GLsizei numIndices;

	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
};
