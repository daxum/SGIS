#pragma once

#include "combinedGl.h"

class Texture {
	friend class TextureManager;
public:
	const GLuint id;

	Texture() = delete;

	void use(GLenum textureUnit = GL_TEXTURE0) const;

private:
	Texture(GLuint id);
};
