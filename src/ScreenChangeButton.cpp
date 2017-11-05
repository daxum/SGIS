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

#include "ScreenChangeButton.hpp"

using namespace std;

ScreenChangeButton::ScreenChangeButton(ScreenManager& manager, float x, float y, float width, float height, string screen, string texture) :
	Button(x, y, width, height, texture),
	screenManager(manager),
	texName(texture),
	screenName(screen) {

}

void ScreenChangeButton::onHoverStart() {
	texture = texName + "_hover";
}

void ScreenChangeButton::onHoverStop() {
	texture = texName;
}

void ScreenChangeButton::onPress() {
	texture = texName + "_press";
}

void ScreenChangeButton::onRelease() {
	screenManager.pushScreenStack();
	screenManager.openScreen(screenName);
	texture = texName;
}
