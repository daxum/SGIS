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
#include "combinedGl.h"
#include "arena.hpp"
#include "Gui.hpp"
#include "ScreenChangeButton.hpp"
#include "BackButton.hpp"

Game::Game() : renderer(this) {
	Gui* mainMenu = new Gui(screenManager, settings, "main");
	mainMenu->setBackground("main_menu");
	mainMenu->addButton(new ScreenChangeButton(screenManager, 0.0f, 0.5f, 1.0f, 0.5625f, "arena", "button"));
	mainMenu->addButton(new BackButton(screenManager, 0.0f, -0.5f, 1.0f, 0.5625f, "button"));

	screenManager.addScreen("main", mainMenu);
	screenManager.addScreen("arena", new Arena(screenManager, settings, "arena", keyTracker));

	screenManager.pushScreenStack();
	screenManager.openScreen("main");
}

Game::~Game() {

}

bool Game::shouldExit() const {
	return renderer.windowClosed() || screenManager.shouldExit();
}

void Game::update() {
	screenManager.update();
}

void Game::render(float partialTicks) {
	renderer.beginDraw();

	screenManager.render(renderer, partialTicks);

	renderer.finishDraw();
}

void Game::handleKeyboardInput(int key, int scanCode, int action) {
	keyTracker.setPressed(scanCode, action != GLFW_RELEASE);

	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		screenManager.onKeyPress(scanCode);
	}
}

void Game::handleMouseScroll(double offset) {
	screenManager.onMouseScroll(offset);
}

void Game::handleMouseMove(double x, double y) {
	float normX = (float) (x / renderer.getViewWidth() * 2.0 - 1.0);
	float normY = -(float) (y / renderer.getViewHeight() * 2.0 - 1.0);

	screenManager.onMouseMove(normX, normY);
}

void Game::handleMouseClick(int button, int action) {
	screenManager.onMouseClick(button, action);
}

void Game::setViewPort(int width, int height) {
	renderer.setViewPort(width, height);
}

void Game::onExit() {
	//Why is this function a thing?
}
