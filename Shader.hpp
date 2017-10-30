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

