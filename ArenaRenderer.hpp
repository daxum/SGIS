#pragma once

#include "combinedGl.h"
#include "Shader.hpp"
#include "RenderManager.hpp"

struct Square;
class Arena;

class ArenaRenderer {
public:
	constexpr static float MIN_CAMERA_HEIGHT = 5.0f;
	constexpr static float MAX_CAMERA_HEIGHT = 300.0f;
	constexpr static float MAX_CAMERA_VELOCITY = 5.0f;

	ArenaRenderer(Arena* arena);
	~ArenaRenderer();

	void updateCamera();
	void increaseCameraVelocity(float amount);
	void render(RenderManager& renderer, float partialTicks);

	float getCameraHeight();

private:
	Arena* arena;

	float prevCameraHeight;
	float cameraHeight;
	float cameraVelocity;

	GLuint squareVbo;
	GLuint borderEbo;
	GLuint squareVao;

	void renderSquare(RenderManager& renderer, const Shader* shader, const Square& toAdd, float partialTicks);
	void initGraphics();
};
