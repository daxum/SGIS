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

#include "Buttons.hpp"
#include "SquareSpawner.hpp"
#include "ScoreUpdater.hpp"
#include "GuiCamera.hpp"
#include "ArenaGenerator.hpp"
#include "ScreenComponents.hpp"
#include "Names.hpp"
#include "SquareWorldState.hpp"

namespace {
	void createGameWorld(Screen* current) {
		std::shared_ptr<SquareState> playerState;
		std::shared_ptr<Screen> world = ArenaGenerator::generateArena(current->getDisplay(), true, 1000, &playerState);

		current->getDisplay().pushScreen(world);

		//Add hud thing
		std::shared_ptr<Screen> hud = std::make_shared<Screen>(current->getDisplay(), false);
		hud->setState<EmptyScreenState>();

		hud->addComponentManager<RenderComponentManager>();
		hud->addComponentManager<UpdateComponentManager>();

		//Score tracker for the player
		std::shared_ptr<Object> score = std::make_shared<Object>();

		score->addComponent<ScoreUpdater>(playerState);
		score->addComponent<TextComponent>(TextMeshInfo{FONT_TEX, U"Score: 0", GENERIC_VERTEX_BUFFER, GENERIC_INDEX_BUFFER, TEXT_FORMAT}, TEXT_MAT);
		std::shared_ptr<TextComponent> scoreText = score->getComponent<TextComponent>();
		scoreText->fitToBox(glm::vec2(380, 380));

		score->addComponent<GuiComponent>(glm::vec3(0.0, 1080.0, -1.5));

		hud->addObject(score);
		hud->setCamera(std::make_shared<GuiCamera>());

		current->getDisplay().pushOverlay(hud);
	}
}

void StartButton::doButtonAction(Screen* screen) {
	createGameWorld(screen);
}

void RetryButton::doButtonAction(Screen* screen) {
	screen->getDisplay().popScreen();
	createGameWorld(screen);
}
