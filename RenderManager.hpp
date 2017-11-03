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
