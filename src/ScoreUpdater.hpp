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
#include "GameObjectStates.hpp"
#include "TextComponent.hpp"

class ScoreUpdater : public UpdateComponent {
public:
	ScoreUpdater(std::shared_ptr<SquareState> toTrack) : tracked(toTrack), prevScore(0) {}

	void update(Screen* screen) {
		if (tracked->numEaten != prevScore) {
			//Update score
			prevScore = tracked->numEaten;
			lockParent()->getComponent<TextComponent>(TEXT_COMPONENT_NAME)->setText(U"Score: " + TextComponent::convToU32(std::to_string(prevScore)));
		}
	}

private:
	std::shared_ptr<SquareState> tracked;

	unsigned int prevScore;
};
