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

#include <vector>
#include <string>
#include "ScreenManager.hpp"
#include "GameSettings.hpp"
#include "RenderManager.hpp"

class Screen {
public:
	Screen(ScreenManager& manager, GameSettings& settings, std::string screenName) :
		name(screenName), screenManager(manager), settings(settings) {}

	virtual ~Screen() {}

	virtual void onOpen() = 0;
	virtual void onClose() = 0;

	virtual void update() = 0;
	virtual void render(RenderManager& renderer, float partialTicks) = 0;

	virtual bool onKeyPress(int key) { return false; }
	virtual bool onMouseScroll(double offset) { return false; }
	virtual bool onMouseMove(float x, float y) { return false; }
	virtual bool onMouseClick(int button, int action) { return false; }

protected:
	const std::string name;
	ScreenManager& screenManager;
	GameSettings& settings;
};
