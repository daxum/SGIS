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

#include <atomic>

#include "Display/Object.hpp"
#include "AxisAlignedBB.hpp"

enum class ObjectType {
	SQUARE,
	WALL,
	BLOCK,
	FLOOR
};

struct GameObjectState : public ObjectState {
	GameObjectState(ObjectType type) : type(type) {}

	const void* getRenderValue(const std::string& name) const override {
		static const glm::vec3 defaultColor(1.0f, 1.0f, 1.0f);

		if (name == "color") {
			return &defaultColor;
		}

		throw std::runtime_error("Bad render value!");
	}

	const ObjectType type;
};

struct SquareState : public GameObjectState {
	SquareState(AxisAlignedBB box, glm::vec3 color, bool player, bool block = false) : GameObjectState(block ? ObjectType::BLOCK : ObjectType::SQUARE), box(box), eaten(false), player(player), numEaten(0), color(color) {}

	const void* getRenderValue(const std::string& name) const override {
		if (name == "color") {
			return &color;
		}

		throw std::runtime_error("Bad render value!");
	}

	AxisAlignedBB box;
	std::atomic<bool> eaten;
	bool player;
	std::atomic<size_t> numEaten;
	glm::vec3 color;
};

struct WallState : public GameObjectState {
	WallState(AxisAlignedBB box) : GameObjectState(ObjectType::WALL), box(box) {}

	AxisAlignedBB box;
};
