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

#include "GuiComponent.hpp"
#include "KeyList.hpp"
#include "Screen.hpp"
#include "DisplayEngine.hpp"

//Pops the screen stack when the set key is pressed.
class BackButton : public GuiComponent {
public:
	BackButton(Object& object, glm::vec3 position, Key key) : GuiComponent(object, position), actionKey(key) {}

	bool onKeyPress(std::shared_ptr<Screen> screen, Key key, KeyAction action) {
		if (key == actionKey && action == KeyAction::RELEASE) {
			screen->getDisplay().popScreen();
			return true;
		}

		return false;
	}

private:
	Key actionKey;
};

class StartButton : public GuiComponent {
public:
	StartButton(Object& object, glm::vec3 position, Key key) : GuiComponent(object, position), actionKey(key) {}

	bool onKeyPress(std::shared_ptr<Screen> screen, Key key, KeyAction action) {
		if (key == actionKey && action == KeyAction::RELEASE) {
			screen->getDisplay().pushScreen(createGameWorld(screen));
			return true;
		}

		return false;
	}

	std::shared_ptr<Screen> createGameWorld(std::shared_ptr<Screen> current);

private:
	Key actionKey;
};
