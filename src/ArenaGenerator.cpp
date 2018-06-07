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

#include "ArenaGenerator.hpp"
#include "ExtraMath.hpp"
#include "RenderComponent.hpp"
#include "ControlledAI.hpp"
#include "RenderComponentManager.hpp"
#include "AIComponentManager.hpp"
#include "PhysicsComponentManager.hpp"
#include "PhysicsComponent.hpp"
#include "UpdateComponentManager.hpp"
#include "SquareSpawner.hpp"
#include "PlanePhysicsObject.hpp"
#include "BoxPhysicsObject.hpp"
#include "SquareCollider.hpp"
#include "GameObjectStates.hpp"
#include "DisplayEngine.hpp"
#include "WorldUpdater.hpp"
#include "SquareWorldState.hpp"
#include "SquareCamera.hpp"
#include "AnimatedCamera.hpp"
#include "Engine.hpp"

namespace {
	void addWall(std::shared_ptr<Screen> world, const AxisAlignedBB& box, const glm::vec3& pos, bool visible = false, const std::string& model = "", const glm::vec3& renderScale = glm::vec3(1.0, 1.0, 1.0)) {
		std::shared_ptr<Object> wall = std::make_shared<Object>();

		wall->addComponent(std::make_shared<PhysicsComponent>(std::make_shared<BoxPhysicsObject>(box, pos, 0.0f)));

		if (visible) {
			wall->addComponent(std::make_shared<RenderComponent>(model, glm::vec3(1.0, 1.0, 1.0), renderScale));
		}

		wall->setState(std::make_shared<WallState>(box));
		world->addObject(wall);
	}
}

std::shared_ptr<Screen> ArenaGenerator::generateArena(DisplayEngine& display, bool playable, unsigned int maxSquares, std::shared_ptr<SquareState>* playerStateOut) {
	std::shared_ptr<Screen> world = std::make_shared<Screen>(display, false);

	//Add component managers to world
	world->addComponentManager(std::make_shared<RenderComponentManager>());
	world->addComponentManager(std::make_shared<AIComponentManager>());
	world->addComponentManager(std::make_shared<PhysicsComponentManager>());
	world->addComponentManager(std::make_shared<UpdateComponentManager>());

	//Create ground
	std::shared_ptr<Object> ground = std::make_shared<Object>();

	ground->addComponent(std::make_shared<PhysicsComponent>(std::make_shared<PlanePhysicsObject>()));
	ground->addComponent(std::make_shared<RenderComponent>("arena", glm::vec3(1.0, 1.0, 1.0), glm::vec3(1000.0, 1.0, 1000.0)));

	world->addObject(ground);

	//Create walls
	AxisAlignedBB nsWall(glm::vec3(-50.0, -6.0, -600.0), glm::vec3(50.0, 6.0, 600.0));
	AxisAlignedBB ewWall(glm::vec3(-600.0, -6.0, -50.0), glm::vec3(600.0, 6.0, 50.0));

	//North, east, south, then west
	addWall(world, ewWall, glm::vec3(0.0, 4.0, -550.0), true, "wall", glm::vec3(500.0, 6.0, 50.0));
	addWall(world, nsWall, glm::vec3(550.0, 4.0, 0.0), true, "wall", glm::vec3(50.0, 6.0, 600.0));
	addWall(world, ewWall, glm::vec3(0.0, 4.0, 550.0), true, "wall", glm::vec3(500.0, 6.0, 50.0));
	addWall(world, nsWall, glm::vec3(-550.0, 4.0, 0.0), true, "wall", glm::vec3(50.0, 6.0, 600.0));

	//Create boundary walls
	AxisAlignedBB ewBound(glm::vec3(-610.0, -50000.0, -50.0), glm::vec3(610.0, 50000.0, 50.0));
	AxisAlignedBB nsBound(glm::vec3(-50.0, -50000.0, -610.0), glm::vec3(50.0, 50000.0, 610.0));

	//Same order as above
	addWall(world, ewBound, glm::vec3(0.0, 49990.0, -650.0));
	addWall(world, nsBound, glm::vec3(650.0, 49990.0, 0.0));
	addWall(world, ewBound, glm::vec3(0.0, 49990.0, 650.0));
	addWall(world, nsBound, glm::vec3(-650.0, 49990.0, 0.0));

	//Create sky
	std::shared_ptr<Object> sky = std::make_shared<Object>();
	sky->addComponent(std::make_shared<RenderComponent>("sky"));

	world->addObject(sky);

	//Set screen state
	std::shared_ptr<SquareWorldState> worldState = std::make_shared<SquareWorldState>();
	world->setState(worldState);

	//Create player and game over objects if world is playable (not demo for main menu)
	//Also change camera based on world playability - follow player or fixed animation
	if (playable) {
		//Player
		std::shared_ptr<Object> square = std::make_shared<Object>();

		std::shared_ptr<SquareState> playerState = std::make_shared<SquareState>(Engine::instance->getModelManager().getModel("square").meshBox, true);
		square->setState(playerState);

		if (playerStateOut) {
			*playerStateOut = playerState;
		}

		square->addComponent(std::make_shared<RenderComponent>("square", glm::vec3(0.1f, 0.9f, 0.1f)));
		square->addComponent(std::make_shared<ControlledAI>());

		AxisAlignedBB playerBox = Engine::instance->getModelManager().getModel("square").meshBox;
		std::shared_ptr<BoxPhysicsObject> playerPhysicsObject = std::make_shared<BoxPhysicsObject>(playerBox);

		std::shared_ptr<PhysicsComponent> physics = std::make_shared<PhysicsComponent>(playerPhysicsObject, std::make_shared<SquareCollider>());
		physics->velocityReduction(false);
		square->addComponent(physics);

		world->addObject(square);

		//Only player in world
		worldState->squareCount = 1;

		//Game over
		std::shared_ptr<Object> gameOverTracker = std::make_shared<Object>();
		gameOverTracker->addComponent(std::make_shared<WorldUpdater>(square));

		world->addObject(gameOverTracker);

		//Camera
		std::shared_ptr<SquareCamera> camera = std::make_shared<SquareCamera>(glm::vec3(0.0f, 135.0f, 0.01f));
		camera->setTarget(square);

		world->setCamera(camera);
	}
	else {
		std::vector<std::pair<glm::vec3, glm::quat>> frames = {
			{{0.0, 300.0, 1500.0}, {0.0, 0.0, 0.0, 0.0}},
			{{0.0, 300.0, 1500.0}, {0.0, 0.0, 0.0, 0.0}},
			{{1500.0, 300.0, 0.0}, {0.0, 0.0, 0.0, 0.0}},
			{{0.0, 300.0, -1500.0}, {0.0, 0.0, 0.0, 0.0}},
			{{-1500.0, 300.0, 0.0}, {0.0, 0.0, 0.0, 0.0}},
			{{0.0, 300.0, 1500.0}, {0.0, 0.0, 0.0, 0.0}},
			{{0.0, 300.0, 1500.0}, {0.0, 0.0, 0.0, 0.0}}
		};

		std::shared_ptr<AnimatedCamera> camera = std::make_shared<AnimatedCamera>(frames, 4800.0f);
		world->setCamera(camera);
	}

	//Create square spawner
	std::shared_ptr<Object> spawner = std::make_shared<Object>();
	spawner->addComponent(std::make_shared<SquareSpawner>(maxSquares));

	world->addObject(spawner);

	return world;
}
