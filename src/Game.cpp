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

void Game::loadTextures(std::shared_ptr<TextureLoader> loader) {
	loader->loadTexture("square", "textures/square.png", Filter::NEAREST, Filter::NEAREST, true);
	loader->loadTexture("arena", "textures/arena.png", Filter::NEAREST, Filter::NEAREST, true);
	loader->loadTexture("wall", "textures/wall.png", Filter::LINEAR, Filter::LINEAR, true);
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

	loader->loadShader("basic", basicInfo);
	loader->loadShader("phong", phongInfo);
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
	quitButton->addComponent(std::make_shared<PhysicsComponent>(std::make_shared<BoxPhysicsObject>(Engine::instance->getModelManager().getModel("square").meshBox, glm::vec3(0.0, -0.5, 0.0), 0.0f)));

	startButton->addComponent(std::make_shared<StartButton>(Key::ENTER));
	startButton->addComponent(std::make_shared<RenderComponent>("button", glm::vec3(0.0, 1.0, 0.0)));
	startButton->addComponent(std::make_shared<PhysicsComponent>(std::make_shared<BoxPhysicsObject>(Engine::instance->getModelManager().getModel("square").meshBox, glm::vec3(0.0, 0.5, 0.0), 0.0f)));

	//Add buttons to menu.
	mainMenu->addObject(quitButton);
	mainMenu->addObject(startButton);

	//Set camera
	std::static_pointer_cast<DefaultCamera>(mainMenu->getCamera())->pos = glm::vec3(0.0, 0.0, -10.0);

	display.pushScreen(mainMenu);
}
