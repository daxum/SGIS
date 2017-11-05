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

struct AxisAlignedBB {
	float minX;
	float minY;
	float maxX;
	float maxY;

	AxisAlignedBB(float minX, float minY, float maxX, float maxY) :
		minX(minX),
		minY(minY),
		maxX(maxX),
		maxY(maxY) {}

	inline bool intersects(const AxisAlignedBB& other) const {
		return minX < other.maxX && maxX > other.minX && minY < other.maxY && maxY > other.minY;
	}

	inline bool contains(const AxisAlignedBB& other) const {
		return maxX >= other.maxX && minX <= other.minX && minY <= other.minY && maxY >= other.maxY;
	}

	inline void translate(float x, float y) {
		minX += x;
		maxX += x;
		minY += y;
		maxY += y;
	}

	inline void getCenter(float& centerX, float& centerY) const {
		centerX = (minX + maxX) / 2.0f;
		centerY = (minY + maxY) / 2.0f;
	}

	inline float getArea() const {
		return (maxX - minX) * (maxY - minY);
	}

	inline void scale(float x, float y) {
		float centerX = (minX + maxX) / 2.0f;
		float centerY = (minY + maxY) / 2.0f;

		float xdiff = (maxX - centerX) * x - (maxX - centerX);
		float ydiff = (maxY - centerY) * y - (maxY - centerY);

		minX -= xdiff;
		minY -= ydiff;
		maxX += xdiff;
		maxY += ydiff;
	}

	inline float xLength() const {
		return maxX - minX;
	}

	inline float yLength() const {
		return maxY - minY;
	}
};
