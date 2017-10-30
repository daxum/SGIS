#pragma once

#include <unordered_map>
#include "Texture.hpp"

class TextureManager {
public:
	~TextureManager();

	const Texture& getTexture(std::string name) const;

	const Texture& loadTexture(std::string name, GLenum magFilter = GL_LINEAR, GLenum minFilter = GL_LINEAR, std::string path = "", std::string extension = ".png");

private:
	std::unordered_map<std::string, Texture> textures;
};

