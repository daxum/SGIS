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

#include "Object.hpp"

enum class ObjectType {
	SQUARE,
	WALL
};

struct GameObjectState : public ObjectState {
	GameObjectState(ObjectType type) : type(type) {}

	const ObjectType type;
};

struct SquareState : public GameObjectState {
	SquareState(AxisAlignedBB box, bool player) : GameObjectState(ObjectType::SQUARE), box(box), eaten(false), player(player) {}

	AxisAlignedBB box;
	bool eaten;
	bool player;
};

struct WallState : public GameObjectState {
	WallState(AxisAlignedBB box) : GameObjectState(ObjectType::WALL), box(box) {}

	AxisAlignedBB box;
};
