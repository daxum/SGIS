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

#include "Gui.hpp"
#include "combinedGl.h"

using namespace std;

Gui::Gui(ScreenManager& manager, GameSettings& settings, string screenName) :
	Screen(manager, settings, screenName),
	buttons(),
	activeButton(nullptr),
	mouseDownButton(nullptr),
	hasBackground(false),
	background("") {

}

Gui::~Gui() {
	for (const Button* b : buttons) {
		delete b;
	}
}

void Gui::onOpen() {

}

void Gui::onClose() {
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
				mouseDownButton = nullptr;
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

void Gui::addButton(Button* button) {
	buttons.push_back(button);
}
