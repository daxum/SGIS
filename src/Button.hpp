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
#include "RenderManager.hpp"

class Button {
public:
	Button(float centerX, float centerY, float width, float height, std::string textureName);

	virtual ~Button() {}

	virtual void onHoverStart() = 0;
	virtual void onHoverStop() = 0;
	virtual void onPress() = 0;
	virtual void onRelease() = 0;

	virtual void update() {}
	virtual void render(RenderManager& renderer, float partialTicks);

	virtual bool isWithin(float mouseX, float mouseY);

protected:
	float x;
	float y;
	float width;
	float height;
	std::string texture;
};
