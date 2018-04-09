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
#include "PhysicsComponent.hpp"

void ControlledAI::update(Screen* screen) {
	glm::vec3 newVelocity;
	float speed = 80.0f;

	if (screen->isKeyPressed(Key::A)) {
		newVelocity.x -= speed;
	}

	//Yes, "north" is negative z. I don't like it either.
	if (screen->isKeyPressed(Key::W)) {
		newVelocity.z -= speed;
	}

	if (screen->isKeyPressed(Key::D)) {
		newVelocity.x += speed;
	}

	if (screen->isKeyPressed(Key::S)) {
		newVelocity.z += speed;
	}

	parent.getComponent<PhysicsComponent>(PHYSICS_COMPONENT_NAME)->setVelocity(newVelocity);
}
