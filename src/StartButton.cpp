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

#include "Buttons.hpp"

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
#include "WorldUpdater.hpp"
#include "SquareWorldState.hpp"
#include "SquareCamera.hpp"
#include "Engine.hpp"

std::shared_ptr<Screen> StartButton::createGameWorld(Screen* current) {
	std::shared_ptr<Screen> world = std::make_shared<Screen>(current->getDisplay(), false);

	//Add component managers to world
	world->addComponentManager(std::make_shared<RenderComponentManager>());
	world->addComponentManager(std::make_shared<AIComponentManager>());
	world->addComponentManager(std::make_shared<PhysicsComponentManager>());
	world->addComponentManager(std::make_shared<UpdateComponentManager>());

	//Create ground
	std::shared_ptr<Object> ground = std::make_shared<Object>();

	ground->addComponent(std::make_shared<PhysicsComponent>(std::make_shared<PlanePhysicsObject>()));
	ground->addComponent(std::make_shared<RenderComponent>("arena", glm::vec3(1.0, 1.0, 1.0), glm::vec3(1000.0, 1.0, 1000.0)));

	//Create walls
	std::shared_ptr<Object> northWall = std::make_shared<Object>();
	std::shared_ptr<Object> eastWall = std::make_shared<Object>();
	std::shared_ptr<Object> southWall = std::make_shared<Object>();
	std::shared_ptr<Object> westWall = std::make_shared<Object>();

	AxisAlignedBB nsWall(glm::vec3(-50.0, -6.0, -600.0), glm::vec3(50.0, 6.0, 600.0));
	AxisAlignedBB ewWall(glm::vec3(-600.0, -6.0, -50.0), glm::vec3(600.0, 6.0, 50.0));

	northWall->addComponent(std::make_shared<PhysicsComponent>(std::make_shared<BoxPhysicsObject>(ewWall, glm::vec3(0.0, 4.0, -550.0), 0.0f)));
	eastWall->addComponent(std::make_shared<PhysicsComponent>(std::make_shared<BoxPhysicsObject>(nsWall, glm::vec3(550.0, 4.0, 0.0), 0.0f)));
	southWall->addComponent(std::make_shared<PhysicsComponent>(std::make_shared<BoxPhysicsObject>(ewWall, glm::vec3(0.0, 4.0, 550.0), 0.0f)));
	westWall->addComponent(std::make_shared<PhysicsComponent>(std::make_shared<BoxPhysicsObject>(nsWall, glm::vec3(-550.0, 4.0, 0.0), 0.0f)));

	northWall->addComponent(std::make_shared<RenderComponent>("wall", glm::vec3(1.0, 1.0, 1.0), glm::vec3(500.0, 6.0, 50.0)));
	eastWall->addComponent(std::make_shared<RenderComponent>("wall", glm::vec3(1.0, 1.0, 1.0), glm::vec3(50.0, 6.0, 600.0)));
	southWall->addComponent(std::make_shared<RenderComponent>("wall", glm::vec3(1.0, 1.0, 1.0), glm::vec3(500.0, 6.0, 50.0)));
	westWall->addComponent(std::make_shared<RenderComponent>("wall", glm::vec3(1.0, 1.0, 1.0), glm::vec3(50.0, 6.0, 600.0)));

	northWall->setState(std::make_shared<WallState>(ewWall));
	eastWall->setState(std::make_shared<WallState>(nsWall));
	southWall->setState(std::make_shared<WallState>(ewWall));
	westWall->setState(std::make_shared<WallState>(nsWall));

	//Create boundary walls
	std::shared_ptr<Object> nBound = std::make_shared<Object>();
	std::shared_ptr<Object> eBound = std::make_shared<Object>();
	std::shared_ptr<Object> sBound = std::make_shared<Object>();
	std::shared_ptr<Object> wBound = std::make_shared<Object>();

	AxisAlignedBB ewBound(glm::vec3(-610.0, -50000.0, -50.0), glm::vec3(610.0, 50000.0, 50.0));
	AxisAlignedBB nsBound(glm::vec3(-50.0, -50000.0, -610.0), glm::vec3(50.0, 50000.0, 610.0));

	nBound->addComponent(std::make_shared<PhysicsComponent>(std::make_shared<BoxPhysicsObject>(ewBound, glm::vec3(0.0, 49990.0, -650.0), 0.0f)));
	eBound->addComponent(std::make_shared<PhysicsComponent>(std::make_shared<BoxPhysicsObject>(nsBound, glm::vec3(650.0, 49990.0, 0.0), 0.0f)));
	sBound->addComponent(std::make_shared<PhysicsComponent>(std::make_shared<BoxPhysicsObject>(ewBound, glm::vec3(0.0, 49990.0, 650.0), 0.0f)));
	wBound->addComponent(std::make_shared<PhysicsComponent>(std::make_shared<BoxPhysicsObject>(nsBound, glm::vec3(-650.0, 49990.0, 0.0), 0.0f)));

	nBound->setState(std::make_shared<WallState>(ewBound));
	nBound->setState(std::make_shared<WallState>(nsBound));
	nBound->setState(std::make_shared<WallState>(ewBound));
	nBound->setState(std::make_shared<WallState>(nsBound));

	//Create player
	std::shared_ptr<Object> square = std::make_shared<Object>();
	square->setState(std::make_shared<SquareState>(Engine::instance->getModelManager().getModel("square").meshBox, true));

	square->addComponent(std::make_shared<RenderComponent>("square", glm::vec3(0.1f, 0.9f, 0.1f)));
	square->addComponent(std::make_shared<ControlledAI>());
	std::shared_ptr<PhysicsComponent> physics = std::make_shared<PhysicsComponent>(std::make_shared<BoxPhysicsObject>(Engine::instance->getModelManager().getModel("square").meshBox), std::make_shared<SquareCollider>());
	physics->velocityReduction(false);
	square->addComponent(physics);

	//Create update objects
	std::shared_ptr<Object> spawner = std::make_shared<Object>();
	spawner->addComponent(std::make_shared<SquareSpawner>());

	std::shared_ptr<Object> gameOverTracker = std::make_shared<Object>();
	gameOverTracker->addComponent(std::make_shared<WorldUpdater>(square));

	//Add objects to screen
	world->addObject(ground);
	world->addObject(square);
	world->addObject(northWall);
	world->addObject(eastWall);
	world->addObject(southWall);
	world->addObject(westWall);
	world->addObject(nBound);
	world->addObject(eBound);
	world->addObject(sBound);
	world->addObject(wBound);
	world->addObject(spawner);
	world->addObject(gameOverTracker);

	//Set screen state
	std::shared_ptr<SquareWorldState> worldState = std::make_shared<SquareWorldState>();

	//Only player in world
	worldState->squareCount = 1;

	world->setState(worldState);

	//Set camera
	std::shared_ptr<SquareCamera> camera = std::make_shared<SquareCamera>(glm::vec3(0.0f, 135.0f, 0.01f));
	camera->setTarget(square);

	world->setCamera(camera);

	return world;
}
