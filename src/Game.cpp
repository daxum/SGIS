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

#include "Game.hpp"
#include "Screen.hpp"
#include "RenderComponent.hpp"
#include "RenderComponentManager.hpp"
#include "Engine.hpp"
#include "GuiComponentManager.hpp"
#include "Buttons.hpp"
#include "PhysicsComponentManager.hpp"
#include "BoxPhysicsObject.hpp"
#include "TextComponent.hpp"

void Game::loadTextures(std::shared_ptr<TextureLoader> loader) {
	loader->loadTexture("square", "textures/square.png", Filter::NEAREST, Filter::NEAREST, true);
	loader->loadTexture("arena", "textures/arena.png", Filter::NEAREST, Filter::NEAREST, true);
	loader->loadTexture("wall", "textures/wall.png", Filter::LINEAR, Filter::LINEAR, true);
	loader->loadFont("font", {"fonts/DejaVuSans.ttf"}, U"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789:;'\".,-!", 90);
}

void Game::loadModels(ModelLoader& loader) {
	//These should really be in material files, but the object loader is having problems with non-absolute paths...
	LightInfo squareLight = {
		glm::vec3(0.25, 0.25, 0.25),
		glm::vec3(1.0, 1.0, 1.0),
		200.0f
	};

	LightInfo arenaLight = {
		glm::vec3(0.27, 0.27, 0.27),
		glm::vec3(0.9, 0.9, 0.9),
		80.0f
	};

	LightInfo wallLight = {
		glm::vec3(0.05, 0.05, 0.05),
		glm::vec3(0.5, 0.5, 0.5),
		200.0f
	};

	loader.loadModel("square", "models/square.obj", "square", "phong", squareLight);
	loader.loadModel("button", "models/square.obj", "square", "basic", squareLight);
	loader.loadModel("arena", "models/arena.obj", "arena", "phong", arenaLight);
	loader.loadModel("wall", "models/cube.obj", "wall", "phong", wallLight);
}

void Game::loadShaders(std::shared_ptr<ShaderLoader> loader) {
	ShaderInfo basicInfo;
	basicInfo.vertex = "shaders/glsl/generic.vert";
	basicInfo.fragment = "shaders/glsl/basic.frag";
	basicInfo.modelView = true;
	basicInfo.projection = true;
	basicInfo.color = true;
	basicInfo.tex0 = true;

	ShaderInfo phongInfo;
	phongInfo.vertex = "shaders/glsl/generic.vert";
	phongInfo.fragment = "shaders/glsl/blinnPhong.frag";
	phongInfo.modelView = true;
	phongInfo.projection = true;
	phongInfo.color = true;
	phongInfo.tex0 = true;
	phongInfo.lightDir = true;
	phongInfo.lighting = true;

	ShaderInfo textInfo;
	textInfo.vertex = "shaders/glsl/text.vert";
	textInfo.fragment = "shaders/glsl/text.frag";
	textInfo.modelView = true;
	textInfo.projection = true;
	textInfo.color = true;
	textInfo.tex0 = true;

	loader->loadShader("basic", basicInfo);
	loader->loadShader("phong", phongInfo);
	loader->loadShader("text", textInfo);
}

void Game::loadScreens(DisplayEngine& display) {
	std::shared_ptr<Screen> mainMenu = std::make_shared<Screen>(display, false);

	//Add component managers.
	mainMenu->addComponentManager(std::make_shared<RenderComponentManager>());
	mainMenu->addComponentManager(std::make_shared<GuiComponentManager>(mainMenu.get()));
	mainMenu->addComponentManager(std::make_shared<PhysicsComponentManager>());

	//Create buttons.
	std::shared_ptr<Object> quitButton = std::make_shared<Object>();
	std::shared_ptr<Object> startButton = std::make_shared<Object>();

	quitButton->addComponent(std::make_shared<BackButton>(Key::ESCAPE));
	quitButton->addComponent(std::make_shared<RenderComponent>("button", glm::vec3(0.9, 0.1, 0.0)));
	quitButton->addComponent(std::make_shared<PhysicsComponent>(std::make_shared<BoxPhysicsObject>(Engine::instance->getModelManager().getModel("square").meshBox, glm::vec3(0.0, -0.6, 0.0), 0.0f)));

	startButton->addComponent(std::make_shared<StartButton>(Key::ENTER));
	startButton->addComponent(std::make_shared<RenderComponent>("button", glm::vec3(0.0, 1.0, 0.0)));
	startButton->addComponent(std::make_shared<PhysicsComponent>(std::make_shared<BoxPhysicsObject>(Engine::instance->getModelManager().getModel("square").meshBox, glm::vec3(0.0, 0.4, 0.0), 0.0f)));

	//Add a title thingy.
	std::shared_ptr<Object> title = std::make_shared<Object>();

	title->addComponent(std::make_shared<TextComponent>(U"Main Menu", "font", "text", glm::vec3(0.02, 0.02, 0.02)));

	AxisAlignedBB textBox = title->getComponent<TextComponent>(TEXT_COMPONENT_NAME)->getTextBox();
	//For position, doesn't take input.
	title->addComponent(std::make_shared<GuiComponent>(glm::vec3(textBox.min.x, 3.5, 0.0)));

	//Add buttons and title to menu.
	mainMenu->addObject(quitButton);
	mainMenu->addObject(startButton);
	mainMenu->addObject(title);

	//Set camera
	std::static_pointer_cast<DefaultCamera>(mainMenu->getCamera())->pos = glm::vec3(0.0, 0.0, 10.0);

	display.pushScreen(mainMenu);
}
