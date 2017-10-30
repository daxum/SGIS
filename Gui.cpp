#include "Gui.hpp"
#include "combinedGl.h"

using namespace std;

Gui::Gui(ScreenManager& manager, GameSettings& settings, string screenName) :
	Screen(manager, settings, screenName),
	background(""),
	hasBackground(false),
	popOnClose(true),
	activeButton(nullptr),
	mouseDownButton(nullptr) {

}

Gui::~Gui() {
	for (const Button* b : buttons) {
		delete b;
	}
}

void Gui::onOpen() {

}

void Gui::onClose() {
	if (popOnClose) {
		screenManager.popScreenStack();
	}

	activeButton->onHoverStop();
	activeButton = nullptr;
	mouseDownButton = nullptr;
}

void Gui::update() {
	for (Button* b : buttons) {
		b->update();
	}
}

void Gui::render(RenderManager& renderer, float partialTicks) {
	renderer.getSpriteRenderer()->startSpriteBatch();

	if (hasBackground) {
		renderer.getSpriteRenderer()->addSprite(0.0f, 0.0f, 2.0f, 2.0f, background);
	}

	for (Button* b : buttons) {
		b->render(renderer, partialTicks);
	}

	renderer.getSpriteRenderer()->endSpriteBatch();
}

bool Gui::onMouseMove(float x, float y) {
	bool didSomething = false;

	if (activeButton != nullptr) {
		if (!activeButton->isWithin(x, y)) {
			activeButton->onHoverStop();
			activeButton = nullptr;
			didSomething = true;
		}
	}

	if (activeButton == nullptr) {
		for (Button* b : buttons) {
			if (b->isWithin(x, y)) {
				activeButton = b;

				if (mouseDownButton == activeButton) {
					activeButton->onPress();
				}
				else {
					activeButton->onHoverStart();
				}

				didSomething = true;
				break;
			}
		}
	}

	return didSomething;
}

bool Gui::onMouseClick(int button, int action) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			mouseDownButton = activeButton;

			if (activeButton != nullptr) {
				activeButton->onPress();
				return true;
			}
		}
		else if (action == GLFW_RELEASE){
			if (activeButton != nullptr && mouseDownButton == activeButton) {
				activeButton->onRelease();
				return true;
			}

			mouseDownButton = nullptr;
		}
	}

	return false;
}

void Gui::setBackground(string textureName) {
	background = textureName;
	hasBackground = true;
}

void Gui::setPopOnClose(bool shouldPop) {
	popOnClose = shouldPop;
}

void Gui::addButton(Button* button) {
	buttons.push_back(button);
}
