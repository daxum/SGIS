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

	AxisAlignedBB(float minX, float minY, float maxX, float maxY);

	bool intersects(const AxisAlignedBB& other) const;
	bool contains(const AxisAlignedBB& other) const;

	void getCenter(float& centerX, float& centerY) const;
	float getArea() const;
	float xLength() const;
	float yLength() const;

	void translate(float x, float y);
	void scale(float x, float y);

	static AxisAlignedBB interpolate(const AxisAlignedBB& start, const AxisAlignedBB& finish, float percent);
};
