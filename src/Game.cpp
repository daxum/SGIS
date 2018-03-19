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

#include "Game.hpp"
#include "Screen.hpp"
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

void Game::loadTextures(std::shared_ptr<TextureLoader> loader) {
	loader->loadTexture("square", "textures/square.png", Filter::NEAREST, Filter::NEAREST, true);
	loader->loadTexture("arena", "textures/arena.png", Filter::NEAREST, Filter::NEAREST, true);
}

void Game::loadModels(ModelLoader& loader) {
	loader.loadModel("square", "models/square.obj", "square");
	loader.loadModel("arena", "models/arena.obj", "arena");
}

void Game::loadScreens(DisplayEngine& display) {
	//Main menu is actually the world at the moment.
	std::shared_ptr<Screen> mainMenu = std::make_shared<Screen>(display);

	//Add component managers to screen
	mainMenu->addComponentManager(std::make_shared<RenderComponentManager>());
	mainMenu->addComponentManager(std::make_shared<AIComponentManager>());
	mainMenu->addComponentManager(std::make_shared<PhysicsComponentManager>());
	mainMenu->addComponentManager(std::make_shared<UpdateComponentManager>());

	//Add update object
	std::shared_ptr<Object> worldUpdater = std::make_shared<Object>();
	worldUpdater->addComponent(std::make_shared<SquareSpawner>(*(worldUpdater.get())));

	mainMenu->addObject(worldUpdater);

	//Create ground
	std::shared_ptr<Object> ground = std::make_shared<Object>();

	ground->addComponent(std::make_shared<PhysicsComponent>(*ground, std::make_shared<PlanePhysicsObject>()));
	ground->addComponent(std::make_shared<RenderComponent>(*ground, "arena", glm::vec3(1.0, 1.0, 1.0), glm::vec3(1000.0, 1.0, 1000.0)));

	//Create invisible walls
	std::shared_ptr<Object> northWall = std::make_shared<Object>();
	std::shared_ptr<Object> eastWall = std::make_shared<Object>();
	std::shared_ptr<Object> southWall = std::make_shared<Object>();
	std::shared_ptr<Object> westWall = std::make_shared<Object>();

	AxisAlignedBB nsWall = AxisAlignedBB(glm::vec3(-100.0, 0.0, -510.0), glm::vec3(0.0, 10.0, 510.0));
	AxisAlignedBB ewWall = AxisAlignedBB(glm::vec3(-510.0, 0.0, -100.0), glm::vec3(510.0, 10.0, 0.0));

	northWall->addComponent(std::make_shared<PhysicsComponent>(*northWall, std::make_shared<BoxPhysicsObject>(ewWall, glm::vec3(0.0, 0.0, -550.0), 0.0f)));
	eastWall->addComponent(std::make_shared<PhysicsComponent>(*eastWall, std::make_shared<BoxPhysicsObject>(nsWall, glm::vec3(550.0, 0.0, 0.0), 0.0f)));
	southWall->addComponent(std::make_shared<PhysicsComponent>(*southWall, std::make_shared<BoxPhysicsObject>(ewWall, glm::vec3(0.0, 0.0, 550.0), 0.0f)));
	westWall->addComponent(std::make_shared<PhysicsComponent>(*westWall, std::make_shared<BoxPhysicsObject>(nsWall, glm::vec3(-550.0, 0.0, 0.0), 0.0f)));

	//Create test object
	std::shared_ptr<Object> square = std::make_shared<Object>();

	square->addComponent(std::make_shared<RenderComponent>(*square, "square", glm::vec3(0.1f, 0.9f, 0.1f)));
	square->addComponent(std::make_shared<ControlledAI>(*square));
	square->addComponent(std::make_shared<PhysicsComponent>(*square, std::make_shared<BoxPhysicsObject>(display.getModelManager().getModel("square").meshBox)));

	//Add objects
	mainMenu->addObject(ground);
	mainMenu->addObject(square);
	mainMenu->addObject(northWall);
	mainMenu->addObject(eastWall);
	mainMenu->addObject(southWall);
	mainMenu->addObject(westWall);

	//Set camera
	mainMenu->getCamera().setTarget(square);

	//Hack for testing
	mainMenu->getCamera().move(0.0f, 200.0f, 0.01f);

	display.pushScreen(mainMenu);
}
