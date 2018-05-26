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
	BackButton(Key key) : GuiComponent(), actionKey(key) {}

	bool onKeyPress(Screen* screen, Key key, KeyAction action) {
		if (key == actionKey && action == KeyAction::RELEASE) {
			screen->getDisplay().popScreen();
			return true;
		}

		return false;
	}

	void onMouseClick(Screen* screen, MouseButton button, MouseAction action) {
		if (button == MouseButton::LEFT && action == MouseAction::RELEASE) {
			screen->getDisplay().popScreen();
		}
	}

private:
	Key actionKey;
};

class StartButton : public GuiComponent {
public:
	StartButton(Key key) : GuiComponent(), actionKey(key) {}

	bool onKeyPress(Screen* screen, Key key, KeyAction action) {
		if (key == actionKey && action == KeyAction::RELEASE) {
			createGameWorld(screen);
			return true;
		}

		return false;
	}

	void onMouseClick(Screen* screen, MouseButton button, MouseAction action) {
		if (button == MouseButton::LEFT && action == MouseAction::RELEASE) {
			createGameWorld(screen);
		}
	}

	void createGameWorld(Screen* current);

private:
	Key actionKey;
};
