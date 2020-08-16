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

#include "Display/Screen.hpp"

struct SquareWorldState : public ScreenState {
	SquareWorldState() : squareCount(0), lightDir(1.0, 1.0, 0.01) {}

	const void* getRenderValue(const std::string& name) const override {
		if (name == "light") {
			return &lightDir;
		}

		throw std::runtime_error("Bad screen render value!");
	}

	std::atomic<size_t> squareCount;
	glm::vec3 lightDir;
};

struct EmptyScreenState : public ScreenState {
	const void* getRenderValue(const std::string& name) const override {
		static const glm::vec3 light = {0.0f, 0.0f, 0.0f};

		if (name == "light") {
			return &light;
		}

		throw std::runtime_error("Bad screen render value!");
	}
};
