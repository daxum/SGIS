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
