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
	SquareSpawner() : UpdateComponent() {}

	void update(Screen* screen);

private:
	constexpr static unsigned int MAX_SPAWN_PER_TICK = 50;
	constexpr static unsigned int MAX_SQUARES = 1000;
	const float MAX_SQUARE_SIZE = 150.0f;

	std::shared_ptr<Object> makeSquare(const AxisAlignedBB& baseBox);
};
