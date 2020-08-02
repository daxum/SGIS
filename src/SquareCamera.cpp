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

#include "SquareCamera.hpp"

SquareCamera::SquareCamera(glm::vec3 startPos, glm::vec3 startLook, glm::vec3 startUp) :
	pos(startPos),
	look(startLook),
	up(startUp),
	velocity(0.0, 0.0, 0.0),
	near(0.1f),
	far(6000.0f) {

}

void SquareCamera::setProjection() {
	float width = Engine::instance->getWindowInterface().getWindowWidth();
	float height = Engine::instance->getWindowInterface().getWindowHeight();

	projection = glm::perspective(ExMath::PI / 4.0f, width / height, near, far);
}

void SquareCamera::update() {
	if (target) {
		glm::vec3 targetPos = target->getPhysics()->getTranslation();

		glm::vec3 newVelocity = -1.5f * (pos - (targetPos + glm::vec3(0.0, 0.0, 10.0)));

		velocity.x = newVelocity.x;
		velocity.z = newVelocity.z;

		look = targetPos + velocity;
	}

	pos += velocity;

	pos.y = ExMath::clamp(pos.y, 1.0f, 1000.0f);
	look.y = ExMath::clamp(look.y, 1.0f, 1000.0f);

	velocity *= 0.95f;
}

void SquareCamera::setTarget(std::shared_ptr<Object> object) {
	target = object;
}

bool SquareCamera::onEvent(const std::shared_ptr<const Event> event) {
	if (event->type == MouseScrollEvent::EVENT_TYPE) {
		velocity.y -= std::static_pointer_cast<const MouseScrollEvent>(event)->y;

		return true;
	}

	return false;
}
