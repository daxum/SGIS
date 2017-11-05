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

#include "RenderManager.hpp"
#include "GameSettings.hpp"
#include "KeyTracker.hpp"
#include "ScreenManager.hpp"
#include "Screen.hpp"

class Game {
public:
	//The time taken for each tick, in milliseconds
	constexpr static double timeStep = 1000.0 / 60.0;

	Game();
	~Game();

	bool shouldExit() const;

	void update();
	void render(float partialTicks);
	void handleKeyboardInput(int key, int scanCode, int action);
	void handleMouseScroll(double offset);
	void handleMouseMove(double x, double y);
	void handleMouseClick(int button, int action);

	void setViewPort(int width, int height);

	void onExit();

private:
	GameSettings settings;
	KeyTracker keyTracker;
	RenderManager renderer;
	ScreenManager screenManager;
};
