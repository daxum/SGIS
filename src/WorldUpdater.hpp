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

#include "UpdateComponent.hpp"

//Tracks the player to determine when the game should be over.
class WorldUpdater : public UpdateComponent {
public:
	WorldUpdater(Object& object, std::shared_ptr<Object> trackObject) : UpdateComponent(object), tracking(trackObject) {}

	void update(Screen* screen) {
		if (std::static_pointer_cast<SquareState>(tracking->getState())->eaten) {
			//Player got eaten!
			screen->getDisplay().popScreen();
			//TODO: game over screen
		}
	}

private:
	//The object the updater tracks.
	std::shared_ptr<Object> tracking;
};

