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
#include "FlatMap.hpp"
#include "RenderComponent.hpp"
#include "ControlledAI.hpp"
#include "RenderComponentManager.hpp"
#include "AIComponentManager.hpp"

void Game::loadTextures(std::shared_ptr<TextureLoader> loader) {
	loader->loadTexture("square", "textures/square.png", Filter::NEAREST, Filter::NEAREST, true);
	loader->loadTexture("arena", "textures/arena.png", Filter::NEAREST, Filter::NEAREST, true);
}

void Game::loadModels(std::shared_ptr<ModelLoader> loader) {
	loader->loadModel("square", "models/square.obj", "square");
	loader->loadModel("arena", "models/arena.obj", "arena");
}

void Game::loadScreens(DisplayEngine& display) {
	//Main menu is actually the world at the moment.
	std::shared_ptr<Screen> mainMenu = std::make_shared<Screen>(display);

	//Add component managers to screen
	mainMenu->addComponentManager(std::make_shared<RenderComponentManager>());
	mainMenu->addComponentManager(std::make_shared<AIComponentManager>());

	//Create test object
	std::shared_ptr<Object> square = std::make_shared<Object>(glm::vec3(0.0f, 0.0f, 0.0f));

	square->addComponent(RENDER_COMPONENT_NAME, std::make_shared<RenderComponent>(*(square.get()), "square"));
	square->addComponent(AI_COMPONENT_NAME, std::make_shared<ControlledAI>(*(square.get())));

	//Add object and set map
	mainMenu->addObject(square);
	mainMenu->setMap(std::make_shared<FlatMap>(100.0f, "arena"));

	//Hack for testing
	mainMenu->getRenderData().camera.move(0.0f, 130.0f, 0.01f);

	display.pushScreen(mainMenu);
}
