#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ArenaRenderer.hpp"
#include "arena.hpp"
#include "ExtraMath.hpp"
#include "Logger.hpp"

ArenaRenderer::ArenaRenderer(Arena* arena) :
	arena(arena),
	prevCameraHeight(20.0f),
	cameraHeight(40.0f),
	cameraVelocity(0.0f) {

	initGraphics();
}

ArenaRenderer::~ArenaRenderer() {
	glDeleteVertexArrays(1, &squareVao);
	glDeleteBuffers(1, &squareVbo);
	glDeleteBuffers(1, &borderEbo);
}

void ArenaRenderer::updateCamera() {
	prevCameraHeight = cameraHeight;
	cameraHeight += cameraVelocity;
	ExMath::clamp(cameraHeight, MIN_CAMERA_HEIGHT, MAX_CAMERA_HEIGHT);
	cameraVelocity *= 0.9f;
}

void ArenaRenderer::increaseCameraVelocity(float amount) {
	cameraVelocity += amount;
	ExMath::clamp(cameraVelocity, -MAX_CAMERA_VELOCITY, MAX_CAMERA_VELOCITY);
}

float ArenaRenderer::getCameraHeight() {
	return cameraHeight;
}

void ArenaRenderer::render(RenderManager& renderer, float partialTicks) {
	float playerCenter[2] = {0.0f, 0.0f};
	ExMath::interpolateBox(arena->player.prevBox, arena->player.boundingBox, partialTicks).getCenter(playerCenter[0], playerCenter[1]);

	float interpolatedHeight = ExMath::interpolate(prevCameraHeight, cameraHeight, partialTicks);

	renderer.setCameraLook(glm::vec3(playerCenter[0], playerCenter[1], 0.0f));
	renderer.setCameraPos(glm::vec3(playerCenter[0], playerCenter[1] - 1.0f, interpolatedHeight));

	renderer.useTexture("arena");

	const Shader* arenaShader = renderer.getShader("arena");
	arenaShader->use();

	glBindVertexArray(squareVao);

	glm::mat4 model;
	model = glm::scale(model, glm::vec3(arena->sizeX / 2.0f + MAX_CAMERA_HEIGHT, arena->sizeY / 2.0f + MAX_CAMERA_HEIGHT, 0.0));

	arenaShader->setUniformMat4("model", model);
	arenaShader->setUniformMat4("view", renderer.getView());
	arenaShader->setUniformMat4("projection", renderer.getProj());
	arenaShader->setUniformVec3("color", glm::vec3(1.0, 1.0, 1.0));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	renderer.useTexture("square");

	glm::mat4 borderModel;
	borderModel = glm::scale(borderModel, glm::vec3(arena->sizeX / 2.0f, arena->sizeY / 2.0f, 0.0));

	arenaShader->setUniformMat4("model", borderModel);
	arenaShader->setUniformVec3("color", glm::vec3(0.0, 0.0, 0.0));

	glDrawElements(GL_LINE_STRIP, 5, GL_UNSIGNED_BYTE, (void*) 0);

	renderSquare(arenaShader, arena->player, partialTicks);

	for (unsigned int i = 0; i < arena->squareCount; i++) {
		renderSquare(arenaShader, arena->squares[i], partialTicks);
	}

	glBindVertexArray(0);
}

void ArenaRenderer::renderSquare(const Shader* shader, const Square& square, float partialTicks) {
	AxisAlignedBB interpolatedBox = ExMath::interpolateBox(square.prevBox, square.boundingBox, partialTicks);

	float centerX = 0.0f;
	float centerY = 0.0f;

	interpolatedBox.getCenter(centerX, centerY);

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(centerX, centerY, 0.5f));
	model = glm::scale(model, glm::vec3(square.sideLength() / 2.0f, square.sideLength() / 2.0f, 1.0f));

	shader->setUniformMat4("model", model);
	shader->setUniformVec3("color", glm::vec3(square.color[0], square.color[1], square.color[2]));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void ArenaRenderer::initGraphics() {
	float square[] = {
		-1.0f, -1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 0.0f
	};

	unsigned char borderIndices[] = {
		0, 2, 3, 1, 0
	};

	glGenVertexArrays(1, &squareVao);
	glGenBuffers(1, &squareVbo);
	glGenBuffers(1, &borderEbo);

	glBindVertexArray(squareVao);

	glBindBuffer(GL_ARRAY_BUFFER, squareVbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, borderEbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(borderIndices), borderIndices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(0);
}
