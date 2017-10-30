#include <stdexcept>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "TextureManager.hpp"
#include "combinedGl.h"
#include "Logger.hpp"

using namespace std;

namespace {
	const int missingWidth = 2;
	const int missingHeight = 2;
	const int missingChannels = 3;

	unsigned char missingData[14] = {
		0x00, 0x00, 0x00, 0xC8,
		0x00, 0xAA, 0x00, 0x00,
		0xC8, 0x00, 0xAA, 0x00,
		0x00, 0x00
	};
}

TextureManager::~TextureManager() {
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint* namesToDelete = new GLuint[textures.size()];

	unsigned int i = 0;
	for (const auto& tex : textures) {
		namesToDelete[i] = tex.second.id;
		i++;
	}

	glDeleteTextures(textures.size(), namesToDelete);
	delete[] namesToDelete;
}

const Texture& TextureManager::getTexture(string name) const {
	return textures.at(name);
}

const Texture& TextureManager::loadTexture(string name, GLenum magFilter, GLenum minFilter, string path, string extension) {
	Logger::pushDomain("TEXTURE");

	if (textures.count(name) != 0) {
		Logger::warn() << "Skipping duplicate texture \"" << path << name << extension << "\"\n";
	}
	else {
		GLuint texId = 0;
		glGenTextures(1, &texId);

		if (texId == 0) {
			Logger::fatal() << "Could not generate texture\n";
			throw runtime_error("Could not generate texture");
		}

		glBindTexture(GL_TEXTURE_2D, texId);

		int width = 0;
		int height = 0;
		int channels = 0;
		bool loaded = true;

		string filename = "textures/" + path + name + extension;
		unsigned char* imageData = stbi_load(filename.c_str(), &width, &height, &channels, 0);

		if (imageData == nullptr) {
			Logger::error() << "Failed to load texture \"" << path << name << extension << "\"\n";

			imageData = missingData;
			width = missingWidth;
			height = missingHeight;
			channels = missingChannels;
			magFilter = GL_NEAREST;
			minFilter = GL_NEAREST;

			loaded = false;
		}

		if (channels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		}
		else if (channels == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		}
		else {
			Logger::error() << "Unsupported channel count for texture \"" << path << name << extension << "\", defaulting to 3\n";
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		glGenerateMipmap(GL_TEXTURE_2D);

		Texture texture(texId);

		textures.insert(make_pair(name, texture));

		glBindTexture(GL_TEXTURE_2D, 0);

		if (loaded) {
			stbi_image_free(imageData);
			Logger::info() << "Loaded texture \"" << path << name << extension << "\"\n";
		}
	}

	Logger::popDomain();

	return textures.at(name);
}
