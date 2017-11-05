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
#include <vector>
#include "Screen.hpp"
#include "Button.hpp"
#include "RenderManager.hpp"

class Gui : public Screen {
public:
	Gui(ScreenManager& manager, GameSettings& settings, std::string screenName);
	~Gui();

	void onOpen();
	void onClose();

	void update();
	void render(RenderManager& renderer, float partialTicks);

	bool onMouseMove(float x, float y);
	bool onMouseClick(int button, int action);

	void setBackground(std::string textureName);

	void addButton(Button* button);

private:
	std::vector<Button*> buttons;
	Button* activeButton;
	Button* mouseDownButton;

	bool hasBackground;
	std::string background;
};
