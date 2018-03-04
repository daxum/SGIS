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

#include "AIComponent.hpp"

//A simple ai that mindlessly goes in one direction.
class SquareAI : public AIComponent {
public:
	SquareAI(Object& parent, glm::vec3 velocity) : AIComponent(parent), velocity(velocity) {
			parent.setState("velocity", std::make_shared<glm::vec3>(velocity));
	}

	void update(Screen* screen) {
		parent.setState("velocity", std::make_shared<glm::vec3>(velocity));
	}

private:
	glm::vec3 velocity;
};

