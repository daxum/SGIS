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
#include "GameObjectStates.hpp"
#include "SquareWorldState.hpp"

class SquareCollider : public CollisionHandler {
	void handleCollision(Screen* screen, PhysicsComponent* hitObject) {
		//Happens when colliding with floor.
		if (!parent->getParent()->getState() || !hitObject->getParent()->getState()) {
			return;
		}

		//Get parent's state (store later, as it probably won't change?)
		std::shared_ptr<SquareState> parentState = std::static_pointer_cast<SquareState>(parent->getParent()->getState());

		//Get what was hit
		ObjectType hitType = getType(hitObject);

		switch(hitType) {
			case ObjectType::SQUARE: doSquareCollision(screen, hitObject, parentState, std::static_pointer_cast<SquareState>(hitObject->getParent()->getState())); break;
			case ObjectType::WALL: doWallCollision(screen, parentState, hitObject); break;
			case ObjectType::BLOCK: break;
			default: throw 5; //Why not?
		}
	}

private:
	ObjectType getType(PhysicsComponent* comp) {
		return std::static_pointer_cast<GameObjectState>(comp->getParent()->getState())->type;
	}

	void doSquareCollision(Screen* screen, PhysicsComponent* hitObject, std::shared_ptr<SquareState> parentState, std::shared_ptr<SquareState> hitState) {
		//If one has already been eaten, don't check again or else squareCount might get messed up. Also, blocks can't be eaten.
		//The eaten checks here are only coarse-grained for if either object was eaten in a previous physics pass,
		//but before the screen update finished.
		if (parentState->eaten.load(std::memory_order_relaxed) || hitState->eaten.load(std::memory_order_relaxed) || parentState->type == ObjectType::BLOCK || hitState->type == ObjectType::BLOCK) {
			return;
		}

		//Uncomment for invulnerable players
		//if (hitState->player) { return; }

		//Who gets eaten?
		if (parentState->box.xLength() > hitState->box.xLength()) {
			bool expectedEaten = false;
			//If another thread has marked this object as eaten (happens in three-way collisions),
			//don't change it again or else squareCount and numEaten will get messed up.
			if (hitState->eaten.compare_exchange_strong(expectedEaten, true, std::memory_order_relaxed)) {
				screen->removeObject(hitObject->getParent());
				parentState->numEaten.fetch_add(1, std::memory_order_relaxed);

				std::shared_ptr<SquareWorldState> worldState = std::static_pointer_cast<SquareWorldState>(screen->getState());

				worldState->squareCount.fetch_sub(1, std::memory_order_relaxed);
			}
		}
	}

	void doWallCollision(Screen* screen, std::shared_ptr<SquareState> parentState, PhysicsComponent* wall) {
		//Players can hit the wall all they want, and squares can't be re-eaten
		if (parentState->player || parentState->eaten.load(std::memory_order_relaxed)) {
			return;
		}

		AxisAlignedBB wallBox = std::static_pointer_cast<WallState>(wall->getParent()->getState())->box;
		wallBox.translate(wall->getTranslation());

		AxisAlignedBB squareBox = parentState->box;
		squareBox.translate(parent->getTranslation());

		//Squares can also be on top of the wall, they should only be destroyed when hit from the side
		if (squareBox.getCenter().y <= wallBox.max.y) {
			bool expectedEaten = false;

			//Eaten by a wall! Oh, the horror!
			//(Don't re-eat if another thread has already consumed this square)
			if (parentState->eaten.compare_exchange_strong(expectedEaten, true, std::memory_order_relaxed)) {
				screen->removeObject(parent->getParent());

				std::shared_ptr<SquareWorldState> worldState = std::static_pointer_cast<SquareWorldState>(screen->getState());

				worldState->squareCount.fetch_sub(1, std::memory_order_relaxed);
			}
		}
	}
};
