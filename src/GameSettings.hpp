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

//Platform-specific default keybindings
enum X11Keys {
	KEY_ESC = 9,
	KEY_W = 25,
	KEY_P = 33,
	KEY_A = 38,
	KEY_S = 39,
	KEY_D = 40
};

class GameSettings {
public:
	//Controls
	int keyMoveUp = KEY_W;
	int keyMoveDown = KEY_S;
	int keyMoveRight = KEY_D;
	int keyMoveLeft = KEY_A;

	//Menu shortcuts
	int keyPlay = KEY_P;
	int keyClose = KEY_ESC;

	//Arena settings - cannot change in game
	float sizeX = 1000.0f;
	float sizeY = 1000.0f;

	GameSettings() {}

private:
};
