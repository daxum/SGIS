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
#include "Buttons.hpp"
#include "ArenaGenerator.hpp"
#include "ScreenComponents.hpp"
#include "DefaultCamera.hpp"
#include "Shaders.hpp"
#include "Vertex.hpp"

void Game::loadTextures(std::shared_ptr<TextureLoader> loader) {
	loader->loadTexture("square", "textures/square.png", Filter::NEAREST, Filter::NEAREST, true);
	loader->loadTexture("arena", "textures/arena.png", Filter::NEAREST, Filter::NEAREST, true);
	loader->loadTexture("wall", "textures/wall.png", Filter::LINEAR, Filter::LINEAR, true);
	loader->loadFont("font", {"fonts/DejaVuSans.ttf"}, U"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789:;'\".,-!?", 90);
	loader->loadCubeMap("skybox", std::vector<std::string>(6, "textures/sky.png"), Filter::NEAREST, Filter::NEAREST, true);
}

void Game::loadModels(ModelLoader& loader) {
	//These should really be in material files, but the object loader is having problems with non-absolute paths...
	LightInfo squareLight = {
		glm::vec3(0.25, 0.25, 0.25),
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(1.0, 1.0, 1.0),
		200.0f
	};

	LightInfo arenaLight = {
		glm::vec3(0.27, 0.27, 0.27),
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(0.95, 0.95, 0.95),
		200.0f
	};

	LightInfo wallLight = {
		glm::vec3(0.05, 0.05, 0.05),
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(0.5, 0.5, 0.5),
		200.0f
	};

	loader.loadModel("square", "models/square.obj", "square", "phong", "generic", squareLight);
	loader.loadModel("button", "models/square.obj", "square", "basic", "generic", squareLight);
	loader.loadModel("arena", "models/arena.obj", "arena", "phong", "generic", arenaLight);
	loader.loadModel("wall", "models/cube.obj", "wall", "phong", "generic", wallLight);
	loader.loadModel("sky", "models/cube_cw.obj", "skybox", "cubemap", "generic", squareLight, false);
}

void Game::loadShaders(std::shared_ptr<ShaderLoader> loader) {
	loader->createBuffer("generic", VertexBufferInfo{{
		{VERTEX_ELEMENT_POSITION, VertexElementType::VEC3},
		{VERTEX_ELEMENT_NORMAL, VertexElementType::VEC3},
		{VERTEX_ELEMENT_TEXTURE, VertexElementType::VEC2}},
		BufferUsage::DEDICATED_LAZY,
		1048576}
	);

	loader->createBuffer("text", VertexBufferInfo{{
		{VERTEX_ELEMENT_POSITION, VertexElementType::VEC2},
		{VERTEX_ELEMENT_TEXTURE, VertexElementType::VEC2}},
		BufferUsage::DEDICATED_SINGLE,
		1048576}
	);

	ShaderInfo basicInfo;
	basicInfo.vertex = "shaders/glsl/generic.vert";
	basicInfo.fragment = "shaders/glsl/basic.frag";
	basicInfo.shaderObject = std::make_shared<BasicShader>();
	basicInfo.pass = RenderPass::OPAQUE;

	ShaderInfo phongInfo;
	phongInfo.vertex = "shaders/glsl/generic.vert";
	phongInfo.fragment = "shaders/glsl/blinnPhong.frag";
	phongInfo.shaderObject = std::make_shared<PhongShader>();
	phongInfo.pass = RenderPass::OPAQUE;

	ShaderInfo textInfo;
	textInfo.vertex = "shaders/glsl/text.vert";
	textInfo.fragment = "shaders/glsl/text.frag";
	textInfo.shaderObject = std::make_shared<BasicShader>();
	textInfo.pass = RenderPass::TRANSLUCENT;

	ShaderInfo skyInfo;
	skyInfo.vertex = "shaders/glsl/cubemap.vert";
	skyInfo.fragment = "shaders/glsl/cubemap.frag";
	skyInfo.shaderObject = std::make_shared<SkyShader>();
	skyInfo.pass = RenderPass::OPAQUE;

	loader->loadShader("basic", basicInfo);
	loader->loadShader("phong", phongInfo);
	loader->loadShader("text", textInfo);
	loader->loadShader("cubemap", skyInfo);
}

void Game::loadScreens(DisplayEngine& display) {
	//Run a "demo" version of the game in the background.
	std::shared_ptr<Screen> demoWorld = ArenaGenerator::generateArena(display, false, 500);
	display.pushScreen(demoWorld);

	//Create main menu.
	std::shared_ptr<Screen> mainMenu = std::make_shared<Screen>(display, false);

	//Add component managers.
	mainMenu->addComponentManager(std::make_shared<RenderComponentManager>());
	mainMenu->addComponentManager(std::make_shared<GuiComponentManager>());
	mainMenu->addComponentManager(std::make_shared<PhysicsComponentManager>());

	//Create buttons.
	std::shared_ptr<Object> quitButton = std::make_shared<Object>();
	std::shared_ptr<Object> startButton = std::make_shared<Object>();

	quitButton->addComponent(std::make_shared<BackButton>(Key::ESCAPE));
	quitButton->addComponent(std::make_shared<RenderComponent>("button", glm::vec3(0.9, 0.1, 0.0)));
	quitButton->addComponent(std::make_shared<PhysicsComponent>(std::make_shared<BoxPhysicsObject>(Engine::instance->getModel("square")->getMesh().getBox(), glm::vec3(0.0, -0.6, 0.0), 0.0f)));

	startButton->addComponent(std::make_shared<StartButton>(Key::ENTER));
	startButton->addComponent(std::make_shared<RenderComponent>("button", glm::vec3(0.0, 1.0, 0.0)));
	startButton->addComponent(std::make_shared<PhysicsComponent>(std::make_shared<BoxPhysicsObject>(Engine::instance->getModel("square")->getMesh().getBox(), glm::vec3(0.0, 0.4, 0.0), 0.0f)));

	//Add a title thingy.
	std::shared_ptr<Object> title = std::make_shared<Object>();

	title->addComponent(std::make_shared<TextComponent>(U"Main Menu", "font", "text", "text", glm::vec3(0.02, 0.02, 0.02)));

	AxisAlignedBB textBox = title->getComponent<TextComponent>(TEXT_COMPONENT_NAME)->getTextBox();
	//For position, doesn't take input.
	title->addComponent(std::make_shared<GuiComponent>(glm::vec3(textBox.min.x, 3.5, 0.0)));

	//Add buttons and title to menu.
	mainMenu->addObject(quitButton);
	mainMenu->addObject(startButton);
	mainMenu->addObject(title);

	//Set camera
	std::static_pointer_cast<DefaultCamera>(mainMenu->getCamera())->pos = glm::vec3(0.0, 0.0, 10.0);

	display.pushOverlay(mainMenu);
}
