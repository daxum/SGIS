#include "arena.hpp"
#include "ExtraMath.hpp"
#include "Logger.hpp"

namespace {
	const float PLAYER_COLOR[3] = {0.1f, 0.9f, 0.1f};
	const float BIGGER_COLOR[3] = {0.9f, 0.06f, 0.06f};
	const float SMALLER_COLOR[3] = {0.95f, 0.95f, 0.04f};
}

Arena::Arena(ScreenManager& manager, GameSettings& settings, std::string screenName, const KeyTracker& keyTracker) :
	Screen(manager, settings, screenName),
	keyTracker(keyTracker),
	renderer(this),
	sizeX(settings.sizeX),
	sizeY(settings.sizeY),
	arenaBox(-settings.sizeX / 2.0f, -settings.sizeY / 2.0f, settings.sizeX / 2.0f, settings.sizeY / 2.0f),
	despawnBox(-settings.sizeX / 2.0f - ArenaRenderer::MAX_CAMERA_HEIGHT,
			   -settings.sizeY / 2.0f - ArenaRenderer::MAX_CAMERA_HEIGHT,
			   settings.sizeX / 2.0f + ArenaRenderer::MAX_CAMERA_HEIGHT,
			   settings.sizeY / 2.0f + ArenaRenderer::MAX_CAMERA_HEIGHT),
	targetArea(settings.sizeX * settings.sizeY * TARGET_PERCENT_AREA),
	player(0.0f, 0.0f, 0.5f, PLAYER_COLOR[0], PLAYER_COLOR[1], PLAYER_COLOR[2]),
	occupiedArea(1.0f),
	squares(),
	squareCount(0),
	eatenScale(0.0f),
	dead(false) {

	squares.reserve(MAX_SQUARES);
}

void Arena::onOpen() {
	spawnInitialSquares();
}

void Arena::onClose() {
	player = Square(0.0f, 0.0f, 0.5f, PLAYER_COLOR[0], PLAYER_COLOR[1], PLAYER_COLOR[2]);
	occupiedArea = player.boundingBox.getArea();
	squareCount = 0;
	eatenScale = 0.0f;
	dead = false;
}

void Arena::update() {
	renderer.updateCamera();

	updateSquare(player, 0.75f, false);
	updatePlayerVelocity();

	for (unsigned int i = 0; i < squareCount; i++) {
		updateSquare(squares[i], 1.0f);
	}

	removeSquares();

	bool playerGrew = checkCollisions();

	removeSquares();

	if (playerGrew) {
		growSquares();
	}

	spawnSquares();

	if (dead) {
		screenManager.closeScreen(name);
		screenManager.popScreenStack();
	}
}

void Arena::updateSquare(Square& square, float friction, bool shouldDespawn) {
	square.prevBox = square.boundingBox;
	square.boundingBox.translate(square.velocity[0], square.velocity[1]);

	if (shouldDespawn) {
		if (!despawnBox.contains(square.boundingBox)) {
			removalList.push_back(square.id);
		}
	}
	else {
		clampToBounds(square);
	}

	square.velocity[0] *= friction;
	square.velocity[1] *= friction;

	if (ExMath::abs(square.velocity[0]) < 0.005f) {
		square.velocity[0] = 0.0f;
	}

	if (ExMath::abs(square.velocity[1]) < 0.005f) {
		square.velocity[1] = 0.0f;
	}
}

void Arena::clampToBounds(Square& square) {
	if (square.boundingBox.minX < arenaBox.minX || square.boundingBox.maxX > arenaBox.maxX) {
		float correction = ExMath::minMagnitude(arenaBox.minX - square.boundingBox.minX, arenaBox.maxX - square.boundingBox.maxX);
		square.boundingBox.translate(correction, 0.0f);
	}

	if (square.boundingBox.minY < arenaBox.minY || square.boundingBox.maxY > arenaBox.maxY) {
		float correction = ExMath::minMagnitude(arenaBox.minY - square.boundingBox.minY, arenaBox.maxY - square.boundingBox.maxY);
		square.boundingBox.translate(0.0f, correction);
	}
}

void Arena::updatePlayerVelocity() {
	if (keyTracker.isKeyPressed(settings.keyMoveUp)) {
		player.velocity[1] += 0.15f;
	}

	if (keyTracker.isKeyPressed(settings.keyMoveDown)) {
		player.velocity[1] -= 0.15f;
	}

	if (keyTracker.isKeyPressed(settings.keyMoveLeft)) {
		player.velocity[0] += 0.15f;
	}

	if (keyTracker.isKeyPressed(settings.keyMoveRight)) {
		player.velocity[0] -= 0.15f;
	}

	ExMath::clamp(player.velocity[0], -MAX_VELOCITY, MAX_VELOCITY);
	ExMath::clamp(player.velocity[1], -MAX_VELOCITY, MAX_VELOCITY);
}

void Arena::removeSquares() {
	for (int i = removalList.size() - 1; i >= 0; i--) {
		removeSquare(squares[removalList[i]]);
	}

	removalList.clear();
}

bool Arena::checkCollisions() {
	bool playerGrew = false;

	for (unsigned int i = 0; i < squareCount; i++) {
		if (player.boundingBox.intersects(squares[i].boundingBox)) {
			if (player.sideLength() >= squares[i].sideLength()) {
				removalList.push_back(squares[i].id);

				eatenScale += squares[i].sideLength() / player.sideLength();
				playerGrew = true;
			}
			else {
				//dead = true;
			}
		}
	}

	if (dead) {
		Logger::error() << "Player died" << "\n";
	}

	return playerGrew;
}

void Arena::growSquares() {
	float newLength = -1000.0f / (0.005f * eatenScale + 10) + 101;
	float scale = newLength / player.sideLength();

	scaleSquare(scale, player);
	Logger::info() << "Player grew to " << player.sideLength() << "!\n";

	for (int i = 0; i < squareCount; i++) {
		scaleSquare(scale, squares[i]);
	}
}

void Arena::scaleSquare(float scale, Square& square) {
	float prevArea = square.boundingBox.getArea();

	square.boundingBox.scale(scale, scale);
	square.prevBox.scale(scale, scale);

	float newArea = square.boundingBox.getArea();

	occupiedArea += newArea - prevArea;
}

void Arena::render(RenderManager& renderManager, float partialTicks) {
	renderer.render(renderManager, partialTicks);
}

bool Arena::onMouseScroll(double offset) {
	renderer.increaseCameraVelocity(-offset);
	return true;
}

void Arena::addSquare(Square square) {
	square.id = squareCount;

	if (squareCount >= squares.size()) {
		squares.push_back(square);
	}
	else {
		squares[squareCount] = square;
	}

	squareCount++;
	occupiedArea += square.boundingBox.getArea();
}

void Arena::removeSquare(Square square) {
	squares[square.id] = squares[squareCount - 1];
	squares[square.id].id = square.id;
	squareCount--;
	occupiedArea -= square.boundingBox.getArea();
}

void Arena::spawnSquares() {
	int spawned = 0;

	while(squareCount < MAX_SQUARES && occupiedArea < targetArea && spawned < MAX_SPAWN_PER_TICK) {
		addSquare(makeSquare(despawnBox, arenaBox, player.sideLength()));
		spawned++;
	}
}

void Arena::spawnInitialSquares() {
	while(squareCount < (MAX_SQUARES / 2) && occupiedArea < targetArea) {
		addSquare(makeSquare(arenaBox, arenaBox, player.sideLength()));
	}
}

Square Arena::makeSquare(const AxisAlignedBB& outer, const AxisAlignedBB& inner, float averageSize) {
	int direction = ExMath::randomInt(0, 3);
	float diameter = ExMath::randomFloat(0.4f * averageSize, ExMath::min(MAX_SQUARE_SIZE, averageSize * 1.5f));
	float centerX;
	float centerY;
	float velocityX = 0.0f;
	float velocityY = 0.0f;

	switch(direction) {
		case 0: //up
			centerX = ExMath::randomFloat(inner.minX, inner.maxX);
			centerY = outer.maxY - diameter - 0.1f;
			velocityY = ExMath::randomFloat(-0.3f, -0.1f);
			break;
		case 1: //right
			centerX = outer.maxX - diameter - 0.1f;
			centerY = ExMath::randomFloat(inner.minY, inner.maxY);
			velocityX = ExMath::randomFloat(-0.3f, -0.1f);
			break;
		case 2: //down
			centerX = ExMath::randomFloat(inner.minX, inner.maxX);
			centerY = outer.minY + diameter + 0.1f;
			velocityY = ExMath::randomFloat(0.1f, 0.3f);
			break;
		case 3: //left
			centerX = outer.minX + diameter + 0.1f;
			centerY = ExMath::randomFloat(inner.minY, inner.maxY);
			velocityX = ExMath::randomFloat(0.1f, 0.3f);
			break;
		default: throw std::out_of_range("Invalid direction " + std::to_string(direction));
	}

	float color[3];

	if (diameter <= averageSize) {
		color[0] = SMALLER_COLOR[0];
		color[1] = SMALLER_COLOR[1];
		color[2] = SMALLER_COLOR[2];
	}
	else {
		color[0] = BIGGER_COLOR[0];
		color[1] = BIGGER_COLOR[1];
		color[2] = BIGGER_COLOR[2];
	}

	return Square(centerX, centerY, diameter / 2.0f, color[0], color[1], color[2], velocityX, velocityY);
}
