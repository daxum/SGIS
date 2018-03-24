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
#include "SquareCollider.hpp"
#include "GameObjectStates.hpp"
#include "WorldUpdater.hpp"
#include "SquareWorldState.hpp"
#include "Engine.hpp"

void Game::loadTextures(std::shared_ptr<TextureLoader> loader) {
	loader->loadTexture("square", "textures/square.png", Filter::NEAREST, Filter::NEAREST, true);
	loader->loadTexture("arena", "textures/arena.png", Filter::NEAREST, Filter::NEAREST, true);
	loader->loadTexture("wall", "textures/wall.png", Filter::LINEAR, Filter::LINEAR, true);
}

void Game::loadModels(ModelLoader& loader) {
	loader.loadModel("square", "models/square.obj", "square", "phong");
	loader.loadModel("arena", "models/arena.obj", "arena", "phong");
	loader.loadModel("wall", "models/cube.obj", "wall", "phong");
}

void Game::loadShaders(std::shared_ptr<ShaderLoader> loader) {
	ShaderInfo basicInfo;
	basicInfo.vertex = "shaders/glsl/generic.vert";
	basicInfo.fragment = "shaders/glsl/basic.frag";
	basicInfo.modelView = true;
	basicInfo.projection = true;
	basicInfo.color = true;
	basicInfo.tex0 = true;

	ShaderInfo phongInfo;
	phongInfo.vertex = "shaders/glsl/generic.vert";
	phongInfo.fragment = "shaders/glsl/blinnPhong.frag";
	phongInfo.modelView = true;
	phongInfo.projection = true;
	phongInfo.color = true;
	phongInfo.tex0 = true;

	loader->loadShader("basic", basicInfo);
	loader->loadShader("phong", phongInfo);
}

void Game::loadScreens(DisplayEngine& display) {
	//Main menu is actually the world at the moment.
	std::shared_ptr<Screen> mainMenu = std::make_shared<Screen>(display);

	//Add component managers to screen
	mainMenu->addComponentManager(std::make_shared<RenderComponentManager>());
	mainMenu->addComponentManager(std::make_shared<AIComponentManager>());
	mainMenu->addComponentManager(std::make_shared<PhysicsComponentManager>());
	mainMenu->addComponentManager(std::make_shared<UpdateComponentManager>());

	//Create ground
	std::shared_ptr<Object> ground = std::make_shared<Object>();

	ground->addComponent(std::make_shared<PhysicsComponent>(*ground, std::make_shared<PlanePhysicsObject>()));
	ground->addComponent(std::make_shared<RenderComponent>(*ground, "arena", glm::vec3(1.0, 1.0, 1.0), glm::vec3(1000.0, 1.0, 1000.0)));

	//Create walls
	std::shared_ptr<Object> northWall = std::make_shared<Object>();
	std::shared_ptr<Object> eastWall = std::make_shared<Object>();
	std::shared_ptr<Object> southWall = std::make_shared<Object>();
	std::shared_ptr<Object> westWall = std::make_shared<Object>();

	AxisAlignedBB nsWall = AxisAlignedBB(glm::vec3(-50.0, -6.0, -510.0), glm::vec3(50.0, 6.0, 510.0));
	AxisAlignedBB ewWall = AxisAlignedBB(glm::vec3(-510.0, -6.0, -50.0), glm::vec3(510.0, 6.0, 50.0));

	northWall->addComponent(std::make_shared<PhysicsComponent>(*northWall, std::make_shared<BoxPhysicsObject>(ewWall, glm::vec3(0.0, 4.0, -550.0), 0.0f, 0.0f)));
	eastWall->addComponent(std::make_shared<PhysicsComponent>(*eastWall, std::make_shared<BoxPhysicsObject>(nsWall, glm::vec3(550.0, 4.0, 0.0), 0.0f, 0.0f)));
	southWall->addComponent(std::make_shared<PhysicsComponent>(*southWall, std::make_shared<BoxPhysicsObject>(ewWall, glm::vec3(0.0, 4.0, 550.0), 0.0f, 0.0f)));
	westWall->addComponent(std::make_shared<PhysicsComponent>(*westWall, std::make_shared<BoxPhysicsObject>(nsWall, glm::vec3(-550.0, 4.0, 0.0), 0.0f, 0.0f)));

	northWall->addComponent(std::make_shared<RenderComponent>(*northWall, "wall", glm::vec3(1.0, 1.0, 1.0), glm::vec3(500.0, 6.0, 50.0)));
	eastWall->addComponent(std::make_shared<RenderComponent>(*eastWall, "wall", glm::vec3(1.0, 1.0, 1.0), glm::vec3(50.0, 6.0, 600.0)));
	southWall->addComponent(std::make_shared<RenderComponent>(*southWall, "wall", glm::vec3(1.0, 1.0, 1.0), glm::vec3(500.0, 6.0, 50.0)));
	westWall->addComponent(std::make_shared<RenderComponent>(*westWall, "wall", glm::vec3(1.0, 1.0, 1.0), glm::vec3(50.0, 6.0, 600.0)));

	northWall->setState(std::make_shared<WallState>(ewWall));
	eastWall->setState(std::make_shared<WallState>(nsWall));
	southWall->setState(std::make_shared<WallState>(ewWall));
	westWall->setState(std::make_shared<WallState>(nsWall));

	//Create test object
	std::shared_ptr<Object> square = std::make_shared<Object>();
	square->setState(std::make_shared<SquareState>(Engine::instance->getModelManager().getModel("square").meshBox, true));

	square->addComponent(std::make_shared<RenderComponent>(*square, "square", glm::vec3(0.1f, 0.9f, 0.1f)));
	square->addComponent(std::make_shared<ControlledAI>(*square));
	square->addComponent(std::make_shared<PhysicsComponent>(*square, std::make_shared<BoxPhysicsObject>(Engine::instance->getModelManager().getModel("square").meshBox), std::make_shared<SquareCollider>()));

	//Create update objects
	std::shared_ptr<Object> spawner = std::make_shared<Object>();
	spawner->addComponent(std::make_shared<SquareSpawner>(*spawner));

	std::shared_ptr<Object> gameOverTracker = std::make_shared<Object>();
	gameOverTracker->addComponent(std::make_shared<WorldUpdater>(*gameOverTracker, square));

	//Add objects to screen
	mainMenu->addObject(ground);
	mainMenu->addObject(square);
	mainMenu->addObject(northWall);
	mainMenu->addObject(eastWall);
	mainMenu->addObject(southWall);
	mainMenu->addObject(westWall);
	mainMenu->addObject(spawner);
	mainMenu->addObject(gameOverTracker);

	//Set screen state
	std::shared_ptr<SquareWorldState> worldState = std::make_shared<SquareWorldState>();

	//Only player in world
	worldState->squareCount = 1;

	mainMenu->setState(worldState);

	//Set camera
	mainMenu->getCamera().setTarget(square);

	//Hack for testing
	mainMenu->getCamera().move(0.0f, 200.0f, 0.01f);

	display.pushScreen(mainMenu);
}
