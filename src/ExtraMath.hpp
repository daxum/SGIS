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

#include "AxisAlignedBB.hpp"

namespace ExMath {
	void clamp(float& value, float minimum, float maximum);

	float abs(float value);

	float interpolate(float start, float finish, float percent);

	AxisAlignedBB interpolateBox(const AxisAlignedBB& start, const AxisAlignedBB& finish, float percent);

	float randomFloat(float min, float max);

	bool randomBool();

	int randomInt(int min, int max);

	double getTimeMillis();

	float min(float val1, float val2);

	float minMagnitude(float val1, float val2);
}
