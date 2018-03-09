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

void SquareSpawner::update(Screen* screen) {
	unsigned int spawned = 0;
	AxisAlignedBB squareBox(screen->getModelManager().getModel("square").meshBox);

	while(squareCount < MAX_SQUARES && occupiedArea < targetArea && spawned < MAX_SPAWN_PER_TICK) {
		screen->addObject(makeSquare(squareBox));
		spawned++;
		squareCount++;
		//TODO: area
	}
}

std::shared_ptr<Object> SquareSpawner::makeSquare(AxisAlignedBB baseBox) {
	//TODO: move to screen state for square growth
	float scaleFactor = 1.0f;

	float posOffset = ExMath::randomFloat(-50.0f, 50.0f);
	float speed = ExMath::randomFloat(0.1f, 0.3f);
	float scale = ExMath::randomFloat(0.4f * scaleFactor, std::min(MAX_SQUARE_SIZE, scaleFactor * 1.5f));
	glm::vec3 translation;
	glm::vec3 velocity;

	switch(ExMath::randomInt(0, 3)) {
		case 0: //moving north
			translation.x = posOffset;
			translation.z = 50.0f;
			velocity.z = -speed;
			break;
		case 1: //moving east
			translation.x = -50.0;
			translation.z = posOffset;
			velocity.x = speed;
			break;
		case 2: //moving south
			translation.x = posOffset;
			translation.z = -50.0f;
			velocity.z = speed;
			break;
		case 3: //moving west
			translation.x = 50.0f;
			translation.z = posOffset;
			velocity.x = -speed;
			break;
		default: throw std::out_of_range("Invalid direction recieved!");
	}

	AxisAlignedBB box = baseBox;
	box.scaleAll(scale);
	box.translate(translation);

	glm::vec3 color = scale <= scaleFactor ? glm::vec3(0.95f, 0.95f, 0.04f) : glm::vec3(0.9f, 0.06f, 0.06f);

	std::shared_ptr<Object> square = std::make_shared<Object>();
	square->addComponent(std::make_shared<PhysicsComponent>(*(square.get()), box));
	square->addComponent(std::make_shared<SquareAI>(*(square.get()), velocity));
	square->addComponent(std::make_shared<RenderComponent>(*(square.get()), "square", color));

	return square;
}
