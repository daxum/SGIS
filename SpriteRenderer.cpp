#include <stdexcept>
#include <glm/glm.hpp>
#include "SpriteRenderer.hpp"
#include "RenderManager.hpp"
#include "Shader.hpp"

using namespace std;

SpriteRenderer::SpriteRenderer(RenderManager* renderer) :
	renderer(renderer),
	active(false),
	aspectX(16.0f),
	aspectY(9.0f) {

}

SpriteRenderer::~SpriteRenderer() {
	glDeleteVertexArrays(1, &spriteVao);
	glDeleteBuffers(1, &spriteVbo);
}

void SpriteRenderer::setTargetAspectRatio(float width, float height) {
	aspectX = width;
	aspectY = height;
}

void SpriteRenderer::startSpriteBatch() {
	if (active) {
		throw runtime_error("Attempted to start duplicate sprite batch!");
	}

	float vw = renderer->getViewWidth();
	float vh = renderer->getViewHeight();
	float multX = aspectX;
	float multY = aspectY;

	while (multX < vw || multY < vh) {
		multX += multX;
		multY += multY;
	}

	viewMultiplierX = multX / vw;
	viewMultiplierY = multY / vh;

	active = true;
}

void SpriteRenderer::addSprite(float x, float y, float width, float height, string texName) {
	if (!active) {
		throw runtime_error("Sprite batching not active!");
	}

	SpriteInfo info;

	info.transX = x * viewMultiplierX;
	info.transY = y * viewMultiplierY;
	info.radX = width / 2.0f * viewMultiplierX;
	info.radY = height / 2.0f * viewMultiplierY;
	info.texName = texName;

	sprites.push(info);
}

void SpriteRenderer::endSpriteBatch() {
	if (!active) {
		throw runtime_error("Attempted to draw sprites when not batching!");
	}

	glBindVertexArray(spriteVao);
	const Shader* shader = renderer->getShader("sprite");
	shader->use();

	while(!sprites.empty()) {
		SpriteInfo info = sprites.front();
		sprites.pop();

		renderer->useTexture(info.texName);

		shader->setUniformVec2("translation", glm::vec2(info.transX, info.transY));
		shader->setUniformVec2("scale", glm::vec2(info.radX, info.radY));

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	glBindVertexArray(0);
	active = false;
}

void SpriteRenderer::initSpriteBuffers() {
	float square[] = {
		-1.0f, -1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &spriteVao);
	glGenBuffers(1, &spriteVbo);

	glBindVertexArray(spriteVao);

	glBindBuffer(GL_ARRAY_BUFFER, spriteVbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(0);
}
