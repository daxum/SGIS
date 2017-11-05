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
#include "ShaderManager.hpp"
#include "TextureManager.hpp"
#include "SpriteRenderer.hpp"
#include "ModelManager.hpp"

class Game;

class RenderManager {
public:
	constexpr static float PI = 3.141592654f;
	constexpr static float DEFAULT_ASPECT_RATIO = 16.0f / 9.0f;

	RenderManager(Game* game);
	~RenderManager();

	void beginDraw();
	void finishDraw();

	glm::mat4 getView();
	glm::mat4 getProj();

	int getViewWidth();
	int getViewHeight();

	SpriteRenderer* getSpriteRenderer();

	void setCameraLook(glm::vec3 look);
	void setCameraPos(glm::vec3 pos);
	glm::vec3 getCameraPos();

	void setViewPort(int width, int height);

	bool windowClosed() const;

	const Shader* getShader(std::string name) const;
	const void renderModel(std::string model, std::string texture, const Shader* shader) const;
	void useTexture(std::string name, GLenum textureUnit = GL_TEXTURE0) const;

private:
	Game* game;
	GLFWwindow* window;
	ShaderManager shaderManager;
	TextureManager textureManager;
	SpriteRenderer spriteRenderer;
	ModelManager modelManager;

	glm::vec3 cameraPos;
	glm::vec3 cameraLook;
	glm::vec3 cameraUp;

	int viewWidth;
	int viewHeight;

	glm::mat4 proj;

	void loadGl();
};
