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

#include "ControlledAI.hpp"

void ControlledAI::update(World* world) {
	glm::vec3 addedVelocity;
	float speed = 0.1f;

	if (world->isKeyPressed(Key::A)) {
		addedVelocity.x -= speed;
	}

	//Yes, "north" is negative z. I don't like it either.
	if (world->isKeyPressed(Key::W)) {
		addedVelocity.z -= speed;
	}

	if (world->isKeyPressed(Key::D)) {
		addedVelocity.x += speed;
	}

	if (world->isKeyPressed(Key::S)) {
		addedVelocity.z += speed;
	}

	parent.addVelocity(addedVelocity);
}
