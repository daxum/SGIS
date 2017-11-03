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

	renderer.setCameraLook(glm::vec3(playerCenter[0], 0.0f, playerCenter[1]));
	renderer.setCameraPos(glm::vec3(playerCenter[0], interpolatedHeight, playerCenter[1] - 10.0f));

	renderer.useTexture("arena");

	const Shader* arenaShader = renderer.getShader("arena");
	arenaShader->use();

	glBindVertexArray(squareVao);

	glm::mat4 model;
	model = glm::scale(model, glm::vec3(arena->sizeX / 2.0f + MAX_CAMERA_HEIGHT, 1.0f, arena->sizeY / 2.0f + MAX_CAMERA_HEIGHT));

	arenaShader->setUniformMat4("model", model);
	arenaShader->setUniformMat4("view", renderer.getView());
	arenaShader->setUniformMat4("projection", renderer.getProj());
	arenaShader->setUniformVec3("color", glm::vec3(1.0, 1.0, 1.0));
	arenaShader->setUniformVec3("cameraPos", renderer.getCameraPos());

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glm::mat4 borderModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.1f, 0.0f));
	borderModel = glm::scale(borderModel, glm::vec3(arena->sizeX / 2.0f, 0.0, arena->sizeY / 2.0f));

	arenaShader->setUniformMat4("model", borderModel);
	arenaShader->setUniformVec3("color", glm::vec3(0.0, 0.0, 0.0));

	glDrawElements(GL_LINE_STRIP, 5, GL_UNSIGNED_BYTE, (void*) 0);

	renderSquare(renderer, arenaShader, arena->player, partialTicks);

	for (unsigned int i = 0; i < arena->squareCount; i++) {
		renderSquare(renderer, arenaShader, arena->squares[i], partialTicks);
	}

	glBindVertexArray(0);
}

void ArenaRenderer::renderSquare(RenderManager& renderer, const Shader* shader, const Square& square, float partialTicks) {
	AxisAlignedBB interpolatedBox = ExMath::interpolateBox(square.prevBox, square.boundingBox, partialTicks);

	float centerX = 0.0f;
	float centerY = 0.0f;

	interpolatedBox.getCenter(centerX, centerY);

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(centerX, 0.315477f * square.sideLength(), centerY));
	model = glm::scale(model, glm::vec3(square.sideLength() / 2.0f, square.sideLength() / 2.0f, square.sideLength() / 2.0f));

	shader->setUniformMat4("model", model);
	shader->setUniformVec3("color", glm::vec3(square.color[0], square.color[1], square.color[2]));

	renderer.renderModel("square", "square", shader);
}

void ArenaRenderer::initGraphics() {
	float square[] = {
		-1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	glBindVertexArray(0);
}
