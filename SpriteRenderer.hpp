#pragma once

#include <queue>
#include <string>
#include "Texture.hpp"
#include "combinedGl.h"

class RenderManager;

struct SpriteInfo {
	float transX;
	float transY;
	float radX;
	float radY;
	std::string texName;
};

class SpriteRenderer {
	friend class RenderManager;
public:
	SpriteRenderer(RenderManager* renderer);
	~SpriteRenderer();

	void startSpriteBatch();

	//x and y are center coordinates, width and height are horizontal and vertical diameters
	void addSprite(float x, float y, float width, float height, std::string texName);

	void endSpriteBatch();

private:
	RenderManager* renderer;
	std::queue<SpriteInfo> sprites;

	GLuint spriteVbo;
	GLuint spriteVao;

	bool active;

	void initSpriteBuffers();
};
