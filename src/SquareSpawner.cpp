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

#include "SquareSpawner.hpp"
#include "ExtraMath.hpp"
#include "Components/PhysicsComponent.hpp"
#include "Components/RenderComponent.hpp"
#include "SquareCollider.hpp"
#include "SquareWorldState.hpp"
#include "Engine.hpp"
#include "Names.hpp"

void SquareSpawner::update(Screen* screen) {
	size_t spawned = 0;

	std::shared_ptr<SquareWorldState> state = screen->getState<SquareWorldState>();

	const AxisAlignedBB& squareBox(Engine::instance->getModelManager().getMesh(SQUARE_MESH, CacheLevel::MEMORY)->getMesh()->getBox());

	while(state->squareCount < maxSquares && spawned < MAX_SPAWN_PER_TICK) {
		screen->addObject(makeSquare(squareBox));

		spawned++;
		state->squareCount++;
	}
}

std::shared_ptr<Object> SquareSpawner::makeSquare(const AxisAlignedBB& baseBox) {
	//TODO: move to screen state for square growth
	float scaleFactor = 1.0f;

	float posOffset = ExMath::randomFloat(-500.0f, 500.0f);
	float speed = ExMath::randomFloat(20.0f, 45.0f);
	float scale = ExMath::randomFloat(0.4f * scaleFactor, std::min(MAX_SQUARE_SIZE, scaleFactor * 1.5f));
	glm::vec3 translation(0.0, 0.0, 0.0);
	glm::vec3 velocity(0.0, 0.0, 0.0);

	switch(ExMath::randomInt(0, 3)) {
		case 0: //moving north
			translation.x = posOffset;
			translation.z = 597.0f;
			velocity.z = -speed;
			break;
		case 1: //moving east
			translation.x = -597.0;
			translation.z = posOffset;
			velocity.x = speed;
			break;
		case 2: //moving south
			translation.x = posOffset;
			translation.z = -597.0f;
			velocity.z = speed;
			break;
		case 3: //moving west
			translation.x = 597.0f;
			translation.z = posOffset;
			velocity.x = -speed;
			break;
		default: throw std::out_of_range("Invalid direction recieved!");
	}

	AxisAlignedBB box = baseBox;
	box.scaleAll(scale);

	//Walls are currently 10 units high
	translation.y = 10.0f + box.yLength() / 2.0f;

	std::shared_ptr<Object> square = std::make_shared<Object>();

	//Is square a block (blue square, can't eat/be eaten)?
	if (ExMath::randomFloat(0.0f, 1.0f) > 0.9f) {
		square->setState<SquareState>(box, glm::vec3(0.0, 0.35, 1.0), false, true);
	}
	else {
		//Big - red, small - yellow
		glm::vec3 color = scale <= scaleFactor ? glm::vec3(0.95f, 0.95f, 0.04f) : glm::vec3(0.9f, 0.06f, 0.06f);
		square->setState<SquareState>(box, color, false);
	}

	PhysicsInfo physInfo = {
		.shape = PhysicsShape::BOX,
		.box = box,
		.pos = translation,
		.mass = 1.0f,
		.friction = 0.5f,
	};

	std::shared_ptr<PhysicsComponent> physics = std::make_shared<PhysicsComponent>(std::make_shared<PhysicsObject>(physInfo), std::make_shared<SquareCollider>());
	physics->velocityReduction(false);
	physics->setVelocity(velocity);

	square->addComponent(physics);
	square->addComponent<RenderComponent>(SQUARE_MAT, SQUARE_MESH, glm::vec3(scale, scale, scale));

	return square;
}
