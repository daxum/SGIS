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
#include "Names.hpp"

//Tracks the player to determine when the game should be over.
class WorldUpdater : public UpdateComponent {
public:
	WorldUpdater(std::shared_ptr<Object> trackObject) : UpdateComponent(), tracking(trackObject) {}

	void update(Screen* screen) {
		std::shared_ptr<SquareState> trackedState = tracking->getState<SquareState>();

		if (trackedState->eaten) {
			//Player got eaten!
			//Pop off score display
			screen->getDisplay().popOverlay();

			//Add game over screen
			std::shared_ptr<Screen> gameOver = std::make_shared<Screen>(screen->getDisplay(), false);
			gameOver->setState<EmptyScreenState>();

			gameOver->addComponentManager<PhysicsManager>();
			gameOver->addComponentManager<GuiManager>();
			gameOver->addComponentManager<RenderManager>();

			TextMeshInfo textInfo = {
				.font = FONT_TEX,
				.text = U"Game Over",
				.vertexBuffer = GENERIC_VERTEX_BUFFER,
				.indexBuffer = GENERIC_INDEX_BUFFER,
				.format = TEXT_FORMAT,
			};

			//Game over message
			std::shared_ptr<Object> message = std::make_shared<Object>();
			message->addComponent<TextComponent>(textInfo, TEXT_MAT);
			std::shared_ptr<TextComponent> gameOverText = message->getComponent<TextComponent>();
			gameOverText->fitToBox(glm::vec2(12.5, 12.5));

			AxisAlignedBB messageBox = message->getComponent<TextComponent>()->getTextBox();
			message->addComponent<GuiComponent>(glm::vec3(messageBox.min.x, 3.2, 0.0));

			gameOver->addObject(message);

			//Final Score
			std::u32string eatenString = TextComponent::convToU32(std::to_string(trackedState->numEaten));
			textInfo.text = U"Final Score: " + eatenString;

			std::shared_ptr<Object> finalScore = std::make_shared<Object>();
			finalScore->addComponent<TextComponent>(textInfo, TEXT_MAT);
			std::shared_ptr<TextComponent> finalScoreText = finalScore->getComponent<TextComponent>();
			finalScoreText->fitToBox(glm::vec2(3.25, 3.25));

			AxisAlignedBB scoreBox = finalScore->getComponent<TextComponent>()->getTextBox();
			finalScore->addComponent<GuiComponent>(glm::vec3(scoreBox.min.x, 0.7, 0.0));

			gameOver->addObject(finalScore);

			//Retry button
			std::shared_ptr<Object> retryButton = std::make_shared<Object>();
			retryButton->setState<ButtonState>(glm::vec3(0.9, 0.9, 0.0));

			retryButton->addComponent<RetryButton>(Key::R);
			retryButton->addComponent<RenderComponent>(BUTTON_MAT, SQUARE_MESH);

			PhysicsInfo buttonInfo = {
				.shape = PhysicsShape::BOX,
				.box = Engine::instance->getModelManager().getMesh(SQUARE_MESH, CacheLevel::MEMORY)->getMesh()->getBox(),
				.pos = glm::vec3(0.0, -0.6, 0.0),
				.mass = 0.0f,
			};

			retryButton->addComponent<PhysicsComponent>(std::make_shared<PhysicsObject>(buttonInfo));
			gameOver->addObject(retryButton);

			//Button to return to main menu
			std::shared_ptr<Object> backButton = std::make_shared<Object>();
			backButton->setState<ButtonState>(glm::vec3(0.9, 0.1, 0.0));

			backButton->addComponent<BackButton>(Key::ESCAPE);
			backButton->addComponent<RenderComponent>(BUTTON_MAT, SQUARE_MESH);

			buttonInfo.pos = glm::vec3(0.0, -1.6, 0.0);

			backButton->addComponent<PhysicsComponent>(std::make_shared<PhysicsObject>(buttonInfo));
			gameOver->addObject(backButton);

			//Set camera
			std::static_pointer_cast<DefaultCamera>(gameOver->getCamera())->setPos(glm::vec3(0.0, 0.0, 10.0));

			screen->getDisplay().pushOverlay(gameOver);

			//Remove this object to prevent opening massive amounts of screens
			screen->removeObject(lockParent());
		}
	}

private:
	//The object the updater tracks.
	std::shared_ptr<Object> tracking;
};

