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
#include "Components/PhysicsComponent.hpp"

void ControlledAI::update(Screen* screen) {
	std::shared_ptr<InputMap> inputMap = screen->getInputMap();

	glm::vec3 newVelocity(0.0, 0.0, 0.0);
	float speed = 80.0f;

	if (inputMap->isKeyPressed(Key::A)) {
		newVelocity.x -= 1.0f;
	}

	//Yes, "north" is negative z. I don't like it either.
	if (inputMap->isKeyPressed(Key::W)) {
		newVelocity.z -= 1.0f;
	}

	if (inputMap->isKeyPressed(Key::D)) {
		newVelocity.x += 1.0f;
	}

	if (inputMap->isKeyPressed(Key::S)) {
		newVelocity.z += 1.0f;
	}

	//Debugging only
	if (inputMap->isKeyPressed(Key::SPACE)) {
		newVelocity.y += 1.0f;
	}

	if (glm::length(newVelocity) != 0.0f) {
		newVelocity = speed * glm::normalize(newVelocity);
	}

	lockParent()->getComponent<PhysicsComponent>(PHYSICS_COMPONENT_NAME)->setVelocity(newVelocity);
}
