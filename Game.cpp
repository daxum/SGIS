#include "Game.hpp"
#include "combinedGl.h"
#include "arena.hpp"
#include "Gui.hpp"
#include "ScreenChangeButton.hpp"

Game::Game() : renderer(this) {
	Gui* mainMenu = new Gui(screenManager, settings, "main");
	mainMenu->setBackground("main_menu");
	mainMenu->addButton(new ScreenChangeButton(screenManager, 0.0f, 0.5f, 1.0f, 0.5625f, "arena", "button"));
	//mainMenu->addButton(new ScreenChangeButton(screenManager, 0.0f, -0.5f, 1.0f, 0.5625f, "arena", "button"));

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
