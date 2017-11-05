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

#include <vector>
#include "AxisAlignedBB.hpp"
#include "RenderManager.hpp"
#include "KeyTracker.hpp"
#include "GameSettings.hpp"
#include "ArenaRenderer.hpp"
#include "Screen.hpp"

struct Square {
	unsigned int id; //Don't touch!
	AxisAlignedBB prevBox;
	AxisAlignedBB boundingBox;
	float velocity[2];
	float color[3];

	Square(float centerX, float centerY, float radius, float red, float green, float blue, float velocityX = 0.0f, float velocityY = 0.0f) :
		id(0),
		prevBox(centerX - radius, centerY - radius, centerX + radius, centerY + radius),
		boundingBox(centerX - radius, centerY - radius, centerX + radius, centerY + radius),
		velocity{velocityX, velocityY},
		color{red, green, blue} {}

	inline float sideLength() const {
		return boundingBox.xLength();
	}
};

class Arena : public Screen {
	friend class ArenaRenderer;
public:
	constexpr static float MIN_ARENA_SIZE = 500.0f;
	constexpr static float MAX_PLAYER_SIZE = 100.0f;
	constexpr static float MAX_SQUARE_SIZE = 1.5f * MAX_PLAYER_SIZE;
	constexpr static float TARGET_PERCENT_AREA = 0.4f;
	constexpr static float MAX_VELOCITY = 2.0f;
	constexpr static float MAX_SPAWN_PER_TICK = 50;
	constexpr static float MAX_SQUARES = 2000;

	Arena(ScreenManager& manager, GameSettings& settings, std::string screenName, const KeyTracker& keyTracker);

	void update();
	void render(RenderManager& renderManager, float partialTicks);

	bool onMouseScroll(double offset);

	void onOpen();
	void onClose();

private:
	const AxisAlignedBB arenaBox;
	const AxisAlignedBB despawnBox;
	const float sizeX;
	const float sizeY;
	const float targetArea;

	const KeyTracker& keyTracker;
	ArenaRenderer renderer;

	float occupiedArea;
	Square player;
	unsigned int squareCount;
	std::vector<Square> squares;
	std::vector<unsigned int> removalList;
	float eatenScale;
	bool dead;

	void updateSquare(Square& square, float friction, bool shouldDespawn = true);
	void clampToBounds(Square& square);
	void updatePlayerVelocity();
	void removeSquares();
	bool checkCollisions();
	void growSquares();
	void scaleSquare(float scale, Square& square);
	void addSquare(Square square);
	void removeSquare(Square square);
	void spawnSquares();
	void spawnInitialSquares();
	Square makeSquare(const AxisAlignedBB& outer, const AxisAlignedBB& inner, float averageSize);
};
