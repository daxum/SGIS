#include "Texture.hpp"

Texture::Texture(GLuint id) : id(id) {}

void Texture::use(GLenum textureUnit) const {
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, id);
}
