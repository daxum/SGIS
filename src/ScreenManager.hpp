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

#include <string>
#include <unordered_map>
#include <vector>
#include <stack>
#include "RenderManager.hpp"

class Screen;

class ScreenManager {
public:
	~ScreenManager();

	void addScreen(std::string name, Screen* screen);

	void openScreen(std::string name);
	void closeScreen(std::string name);

	void pushScreenStack();
	void popScreenStack();

	void update();
	void render(RenderManager& renderer, float partialTicks);

	void onKeyPress(int key);
	void onMouseScroll(double offset);
	void onMouseMove(float x, float y);
	void onMouseClick(int button, int action);

	bool shouldExit() const;

private:
	std::unordered_map<std::string, Screen*> screens;
	std::stack<std::vector<Screen*>> activeScreenStack;
};
