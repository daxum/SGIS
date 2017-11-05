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

#include "Button.hpp"

using namespace std;

Button::Button(float centerX, float centerY, float width, float height, string textureName) :
	x(centerX),
	y(centerY),
	width(width),
	height(height),
	texture(textureName) {

}

void Button::render(RenderManager& renderer, float partialTicks) {
	renderer.getSpriteRenderer()->addSprite(x, y, width, height, texture);
}

bool Button::isWithin(float mouseX, float mouseY) {
	return mouseX >= (x - width / 2.0f) && mouseX <= (x + width / 2.0f) &&
		   mouseY >= (y - height / 2.0f) && mouseY <= (y + height / 2.0f);
}
