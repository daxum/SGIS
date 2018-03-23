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
#include "PhysicsComponent.hpp"
#include "SquareAI.hpp"
#include "RenderComponent.hpp"
#include "BoxPhysicsObject.hpp"
#include "SquareCollider.hpp"
#include "SquareWorldState.hpp"
#include "Engine.hpp"

void SquareSpawner::update(Screen* screen) {
	unsigned int spawned = 0;

	std::shared_ptr<SquareWorldState> state = std::static_pointer_cast<SquareWorldState>(screen->getState());

	const AxisAlignedBB& squareBox(Engine::instance->getModelManager().getModel("square").meshBox);

	while(state->squareCount < MAX_SQUARES && spawned < MAX_SPAWN_PER_TICK) {

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
	glm::vec3 translation;
	glm::vec3 velocity;

	switch(ExMath::randomInt(0, 3)) {
		case 0: //moving north
			translation.x = posOffset;
			translation.z = 500.0f;
			velocity.z = -speed;
			break;
		case 1: //moving east
			translation.x = -500.0;
			translation.z = posOffset;
			velocity.x = speed;
			break;
		case 2: //moving south
			translation.x = posOffset;
			translation.z = -500.0f;
			velocity.z = speed;
			break;
		case 3: //moving west
			translation.x = 500.0f;
			translation.z = posOffset;
			velocity.x = -speed;
			break;
		default: throw std::out_of_range("Invalid direction recieved!");
	}

	AxisAlignedBB box = baseBox;
	box.scaleAll(scale);

	glm::vec3 color = scale <= scaleFactor ? glm::vec3(0.95f, 0.95f, 0.04f) : glm::vec3(0.9f, 0.06f, 0.06f);

	std::shared_ptr<Object> square = std::make_shared<Object>();
	square->setState(std::make_shared<SquareState>(box.xLength()));

	square->addComponent(std::make_shared<PhysicsComponent>(*square, std::make_shared<BoxPhysicsObject>(box, translation), std::make_shared<SquareCollider>()));
	square->addComponent(std::make_shared<SquareAI>(*square, velocity));
	square->addComponent(std::make_shared<RenderComponent>(*square, "square", color, glm::vec3(scale, scale, scale)));

	return square;
}
