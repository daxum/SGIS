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

#include "PhysicsComponent.hpp"
#include "SquareState.hpp"
#include "SquareWorldState.hpp"

class SquareCollider : public CollisionHandler {
	void handleCollision(Screen* screen, PhysicsComponent* hitObject) {
		//This only works because SquareState is the only type of object state.
		if (!parent->getParent()->getState() || !hitObject->getParent()->getState()) {
			return;
		}

		std::shared_ptr<SquareState> parentState = std::static_pointer_cast<SquareState>(parent->getParent()->getState());
		std::shared_ptr<SquareState> hitState = std::static_pointer_cast<SquareState>(hitObject->getParent()->getState());

		if (parentState->size > hitState->size) {
			screen->removeObject(hitObject->getParent());
			hitState->eaten = true;

			std::shared_ptr<SquareWorldState> worldState = std::static_pointer_cast<SquareWorldState>(screen->getState());

			worldState->squareCount--;
		}
	}
};
