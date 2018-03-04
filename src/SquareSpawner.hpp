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

#include "UpdateComponent.hpp"

class SquareSpawner : public UpdateComponent {
public:
	SquareSpawner(Object& object) : UpdateComponent(object), targetArea(4000.0f), squareCount(0), occupiedArea(0.0f) {}

	void update(Screen* screen);

private:
	constexpr static unsigned int MAX_SPAWN_PER_TICK = 50;
	constexpr static unsigned int MAX_SQUARES = 100;
	const float MAX_SQUARE_SIZE = 150.0f;

	const float targetArea;

	//TODO: below two need to be moved to screen state, for proper decrementing.
	unsigned int squareCount;
	float occupiedArea;

	std::shared_ptr<Object> makeSquare(AxisAlignedBB baseBox);
};
