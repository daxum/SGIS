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

#include <stdexcept>

namespace ExMath {
	//Note: Passing constexpr values to functions that take things by reference, like this one, does not work.
	//This can be fixed by doing T(Value), where T is a type and value is the constexpr object, like float(CONST_FLOAT)
	//Should probably find another way to fix later
	template <typename T>
	void clamp(T& value, const T& minimum, const T& maximum) {
		if (minimum >= maximum) {
			throw std::logic_error("Maximum not greater than minimum!");
		}

		if (value > maximum) {
			value = maximum;
		}

		if (value < minimum) {
			value = minimum;
		}
	}

	float interpolate(float start, float finish, float percent);

	float randomFloat(float min, float max);

	bool randomBool();

	int randomInt(int min, int max);

	double getTimeMillis();

	float minMagnitude(float val1, float val2);
}
