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

#include "Buttons.hpp"
#include "DefaultCamera.hpp"
#include "ScreenComponents.hpp"

//Tracks the player to determine when the game should be over.
class WorldUpdater : public UpdateComponent {
public:
	WorldUpdater(std::shared_ptr<Object> trackObject) : UpdateComponent(), tracking(trackObject) {}

	void update(Screen* screen) {
		std::shared_ptr<SquareState> trackedState = std::static_pointer_cast<SquareState>(tracking->getState());

		if (trackedState->eaten) {
			//Player got eaten!
			//Pop off score display
			screen->getDisplay().popOverlay();

			//Add game over screen
			std::shared_ptr<Screen> gameOver = std::make_shared<Screen>(screen->getDisplay(), false);

			gameOver->addComponentManager(std::make_shared<PhysicsComponentManager>());
			gameOver->addComponentManager(std::make_shared<GuiComponentManager>());
			gameOver->addComponentManager(std::make_shared<RenderComponentManager>());

			//Game over message
			std::shared_ptr<Object> message = std::make_shared<Object>();
			message->addComponent(std::make_shared<TextComponent>(U"Game Over", "font", "text", glm::vec3(0.025, 0.025, 0.025)));

			AxisAlignedBB messageBox = message->getComponent<TextComponent>(TEXT_COMPONENT_NAME)->getTextBox();
			message->addComponent(std::make_shared<GuiComponent>(glm::vec3(messageBox.min.x, 3.2, 0.0)));

			gameOver->addObject(message);

			//Final Score
			std::shared_ptr<Object> finalScore = std::make_shared<Object>();
			finalScore->addComponent(std::make_shared<TextComponent>(U"Final Score: " + numToString(trackedState->numEaten), "font", "text", glm::vec3(0.005, 0.005, 0.005)));

			AxisAlignedBB scoreBox = finalScore->getComponent<TextComponent>(TEXT_COMPONENT_NAME)->getTextBox();
			finalScore->addComponent(std::make_shared<GuiComponent>(glm::vec3(scoreBox.min.x, 0.7, 0.0)));

			gameOver->addObject(finalScore);

			//Retry button
			std::shared_ptr<Object> retryButton = std::make_shared<Object>();

			retryButton->addComponent(std::make_shared<RetryButton>(Key::R));
			retryButton->addComponent(std::make_shared<RenderComponent>("button", glm::vec3(0.9, 0.9, 0.0)));
			retryButton->addComponent(std::make_shared<PhysicsComponent>(std::make_shared<BoxPhysicsObject>(Engine::instance->getModelManager().getModel("square").meshBox, glm::vec3(0.0, -0.6, 0.0), 0.0f)));
			gameOver->addObject(retryButton);

			//Button to return to main menu
			std::shared_ptr<Object> backButton = std::make_shared<Object>();

			backButton->addComponent(std::make_shared<BackButton>(Key::ESCAPE));
			backButton->addComponent(std::make_shared<RenderComponent>("button", glm::vec3(0.9, 0.1, 0.0)));
			backButton->addComponent(std::make_shared<PhysicsComponent>(std::make_shared<BoxPhysicsObject>(Engine::instance->getModelManager().getModel("square").meshBox, glm::vec3(0.0, -1.6, 0.0), 0.0f)));
			gameOver->addObject(backButton);

			//Set camera
			std::static_pointer_cast<DefaultCamera>(gameOver->getCamera())->pos = glm::vec3(0.0, 0.0, 10.0);

			screen->getDisplay().pushOverlay(gameOver);

			//Remove this object to prevent opening massive amounts of screens
			screen->removeObject(lockParent());
		}
	}

private:
	//The object the updater tracks.
	std::shared_ptr<Object> tracking;

	/**
	 * Taken from ScoreUpdate.hpp, find better place later.
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

