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
			lockParent()->getComponent<TextComponent>(TEXT_COMPONENT_NAME)->setText(U"Score: " + numToString(prevScore));
		}
	}

private:
	std::shared_ptr<SquareState> tracked;

	unsigned int prevScore;

	/**
	 * This is stupid.
	 */
	std::u32string numToString(unsigned int num) {
		std::string conv = std::to_string(num);
		std::u32string out;
		out.reserve(conv.size());

		for (const char& c : conv) {
			switch(c) {
				case '0': out.push_back(U'0'); break;
				case '1': out.push_back(U'1'); break;
				case '2': out.push_back(U'2'); break;
				case '3': out.push_back(U'3'); break;
				case '4': out.push_back(U'4'); break;
				case '5': out.push_back(U'5'); break;
				case '6': out.push_back(U'6'); break;
				case '7': out.push_back(U'7'); break;
				case '8': out.push_back(U'8'); break;
				case '9': out.push_back(U'9'); break;
				default: out.push_back(U'?'); break;
			}
		}

		return out;
	}
};
