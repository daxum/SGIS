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

#pragma once

#include "KeyList.hpp"
#include "ScreenComponents.hpp"

struct ButtonState : public ObjectState {
	ButtonState(glm::vec3 color) : color(color) {}

	const void* getRenderValue(const std::string& name) const override {
		if (name == "color") {
			return &color;
		}

		throw std::runtime_error("Bad render value!");
	}

	glm::vec3 color;
};

class Button : public GuiComponent {
public:
	Button(Key shortcut) : GuiComponent(), actionKey(shortcut) {}
	virtual ~Button() {}

	bool onKeyPress(Screen* screen, Key key, KeyAction action) {
		if (key == actionKey && action == KeyAction::RELEASE) {
			doButtonAction(screen);
			return true;
		}

		return false;
	}

	void onMouseClick(Screen* screen, MouseButton button, MouseAction action) {
		if (button == MouseButton::LEFT && action == MouseAction::RELEASE) {
			doButtonAction(screen);
		}
	}

	void onHoverStart(Screen* screen) {
		std::shared_ptr<RenderComponent> render = lockParent()->getComponent<RenderComponent>(RENDER_COMPONENT_NAME);

		if (render) {
			render->setScale(render->getScale() + BUTTON_SIZE_CHANGE);
		}
	}

	void onHoverStop(Screen* screen) {
		std::shared_ptr<RenderComponent> render = lockParent()->getComponent<RenderComponent>(RENDER_COMPONENT_NAME);

		if (render) {
			render->setScale(render->getScale() - BUTTON_SIZE_CHANGE);
		}
	}

	virtual void doButtonAction(Screen* screen) = 0;

protected:
	constexpr static const glm::vec3 BUTTON_SIZE_CHANGE = glm::vec3(0.05, 0.05, 0.05);
	Key actionKey;
};

//Pops the screen stack.
class BackButton : public Button {
public:
	BackButton(Key shortcut) : Button(shortcut) {}

	void doButtonAction(Screen* screen) { screen->getDisplay().popScreen(); }
};

//Starts the game.
class StartButton : public Button {
public:
	StartButton(Key shortcut) : Button(shortcut) {}

	void doButtonAction(Screen* screen);
};

//Retry after losing.
class RetryButton : public Button {
public:
	RetryButton(Key shortcut) : Button(shortcut) {}

	void doButtonAction(Screen* screen);
};
