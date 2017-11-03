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
