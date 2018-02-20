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
#include "World.hpp"

void Game::loadTextures(std::shared_ptr<TextureLoader> loader) {
	loader->loadTexture("square", "textures/square.png", Filter::NEAREST, Filter::NEAREST, true);
	loader->loadTexture("arena", "textures/arena.png", Filter::NEAREST, Filter::NEAREST, true);
}

void Game::loadModels(std::shared_ptr<ModelLoader> loader) {
	loader->loadModel("square", "models/square.obj", "square");
}

void Game::loadScreens(DisplayEngine& display) {
	//Main menu is actually the world at the moment.
	std::shared_ptr<World> mainMenu = std::make_shared<World>(display);
	mainMenu->addObject(std::make_shared<Object>("square"));

	//Hack for testing
	mainMenu->getRenderData().camera.move(1.0f, 1.0f, 5.0f);

	display.pushScreen(mainMenu);
}
