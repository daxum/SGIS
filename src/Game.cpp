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
#include "Vertex.hpp"
#include "Names.hpp"
#include "SquareWorldState.hpp"

namespace {
	struct ShaderNames {
		const char* const vertex;
		const char* const fragment;
	};

	struct ShaderPaths {
		ShaderNames basic;
		ShaderNames phong;
		ShaderNames text;
		ShaderNames sky;
	};

	const ShaderPaths glShaders = {
		{"shaders/glsl/generic.vert", "shaders/glsl/basic.frag"},
		{"shaders/glsl/generic.vert", "shaders/glsl/blinnPhong.frag"},
		{"shaders/glsl/text.vert", "shaders/glsl/text.frag"},
		{"shaders/glsl/cubemap.vert", "shaders/glsl/cubemap.frag"}
	};

	const ShaderPaths vkShaders = {
		{"shaders/spirv/generic.vert.spv", "shaders/spirv/basic.frag.spv"},
		{"shaders/spirv/generic.vert.spv", "shaders/spirv/blinnPhong.frag.spv"},
		{"shaders/spirv/text.vert.spv", "shaders/spirv/text.frag.spv"},
		{"shaders/spirv/cubemap.vert.spv", "shaders/spirv/cubemap.frag.spv"}
	};

	const ShaderPaths* const shaderFiles = Game::USE_VULKAN ? &vkShaders : &glShaders;
}

void Game::createRenderObjects(std::shared_ptr<RenderInitializer> renderInit) {
	renderInit->createBuffer(GENERIC_BUFFER, VertexBufferInfo{{
		{VERTEX_ELEMENT_POSITION, VertexElementType::VEC3},
		{VERTEX_ELEMENT_NORMAL, VertexElementType::VEC3},
		{VERTEX_ELEMENT_TEXTURE, VertexElementType::VEC2}},
		BufferUsage::DEDICATED_LAZY,
		1048576
	});

	renderInit->createBuffer(TEXT_BUFFER, VertexBufferInfo{{
		{VERTEX_ELEMENT_POSITION, VertexElementType::VEC2},
		{VERTEX_ELEMENT_TEXTURE, VertexElementType::VEC2}},
		BufferUsage::DEDICATED_SINGLE,
		1048576
	});

	renderInit->addUniformSet(PHONG_SET, UniformSet{
		UniformSetType::MODEL_STATIC,
		3,
		{{UniformType::SAMPLER_2D, SQUARE_TEX, UniformProviderType::MATERIAL, USE_FRAGMENT_SHADER},
		{UniformType::VEC3, "ka", UniformProviderType::MATERIAL, USE_FRAGMENT_SHADER},
		{UniformType::VEC3, "ks", UniformProviderType::MATERIAL, USE_FRAGMENT_SHADER},
		{UniformType::FLOAT, "s", UniformProviderType::MATERIAL, USE_FRAGMENT_SHADER}}
	});

	renderInit->addUniformSet(BASIC_SET, UniformSet{
		UniformSetType::MODEL_STATIC,
		1,
		{{UniformType::SAMPLER_2D, SQUARE_TEX, UniformProviderType::MATERIAL, USE_FRAGMENT_SHADER}}
	});

	renderInit->addUniformSet(CUBE_SET, UniformSet{
		UniformSetType::MODEL_STATIC,
		1,
		{{UniformType::SAMPLER_CUBE, SKY_TEX, UniformProviderType::MATERIAL, USE_FRAGMENT_SHADER}}
	});

	renderInit->addUniformSet(TEXT_SET, UniformSet{
		UniformSetType::MODEL_DYNAMIC,
		1,
		{{UniformType::SAMPLER_2D, FONT_TEX, UniformProviderType::MATERIAL, USE_FRAGMENT_SHADER}}
	});

	renderInit->addUniformSet(SCREEN_SET, UniformSet{
		UniformSetType::PER_SCREEN,
		3,
		{{UniformType::MAT4, "projection", UniformProviderType::CAMERA_PROJECTION, USE_VERTEX_SHADER},
		{UniformType::MAT4, "view", UniformProviderType::CAMERA_VIEW, USE_VERTEX_SHADER},
		{UniformType::VEC3, "light", UniformProviderType::SCREEN_STATE, USE_VERTEX_SHADER}}
	});
}

void Game::loadTextures(std::shared_ptr<TextureLoader> loader) {
	loader->loadTexture(SQUARE_TEX, "textures/square.png", Filter::NEAREST, Filter::NEAREST, true);
	loader->loadTexture(ARENA_TEX, "textures/arena.png", Filter::LINEAR, Filter::LINEAR, true);
	loader->loadTexture(WALL_TEX, "textures/wall.png", Filter::LINEAR, Filter::LINEAR, true);
	loader->loadFont(FONT_TEX, {"fonts/DejaVuSans.ttf"}, U"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789:;'\".,-!?", 191);
	loader->loadCubeMap(SKY_TEX, {"textures/sky_east.png", "textures/sky_west.png", "textures/sky_north.png", "textures/sky_north.png", "textures/sky_south.png", "textures/sky_north.png"}, Filter::LINEAR, Filter::LINEAR, true);
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
		glm::vec3(0.5, 0.7, 0.75),
		200.0f
	};

	LightInfo wallLight = {
		glm::vec3(0.05, 0.05, 0.05),
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(0.5, 0.5, 0.5),
		200.0f
	};

	loader.loadModel(SQUARE_MODEL, "models/square.obj", SQUARE_TEX, PHONG_SHADER, GENERIC_BUFFER, PHONG_SET, squareLight);
	loader.loadModel(BUTTON_MODEL, "models/square.obj", SQUARE_TEX, BASIC_SHADER, GENERIC_BUFFER, BASIC_SET, squareLight);
	loader.loadModel(ARENA_MODEL, "models/arena.obj", ARENA_TEX, PHONG_SHADER, GENERIC_BUFFER, PHONG_SET, arenaLight);
	loader.loadModel(WALL_MODEL, "models/cube.obj", WALL_TEX, PHONG_SHADER, GENERIC_BUFFER, PHONG_SET, wallLight);
	loader.loadModel(SKY_MODEL, "models/cube_cw.obj", SKY_TEX, SKYBOX_SHADER, GENERIC_BUFFER, CUBE_SET, squareLight, false);
}

void Game::loadShaders(std::shared_ptr<ShaderLoader> loader) {
	ShaderInfo basicInfo;
	basicInfo.vertex = shaderFiles->basic.vertex;
	basicInfo.fragment = shaderFiles->basic.fragment;
	basicInfo.pass = RenderPass::OPAQUE;
	basicInfo.buffer = GENERIC_BUFFER;
	basicInfo.uniformSets = {SCREEN_SET, BASIC_SET};
	basicInfo.pushConstants = {{
		{UniformType::MAT4, "modelView", UniformProviderType::OBJECT_MODEL_VIEW, USE_VERTEX_SHADER},
		{UniformType::VEC3, "color", UniformProviderType::OBJECT_STATE, USE_FRAGMENT_SHADER}
	}};

	ShaderInfo phongInfo;
	phongInfo.vertex = shaderFiles->phong.vertex;
	phongInfo.fragment = shaderFiles->phong.fragment;
	phongInfo.pass = RenderPass::OPAQUE;
	phongInfo.buffer = GENERIC_BUFFER;
	phongInfo.uniformSets = {SCREEN_SET, PHONG_SET};
	phongInfo.pushConstants = {{
		{UniformType::MAT4, "modelView", UniformProviderType::OBJECT_MODEL_VIEW, USE_VERTEX_SHADER},
		{UniformType::VEC3, "color", UniformProviderType::OBJECT_STATE, USE_FRAGMENT_SHADER}
	}};

	ShaderInfo textInfo;
	textInfo.vertex = shaderFiles->text.vertex;
	textInfo.fragment = shaderFiles->text.fragment;
	textInfo.pass = RenderPass::TRANSLUCENT;
	textInfo.buffer = TEXT_BUFFER;
	textInfo.uniformSets = {SCREEN_SET, TEXT_SET};
	textInfo.pushConstants = {{
		{UniformType::MAT4, "modelView", UniformProviderType::OBJECT_MODEL_VIEW, USE_VERTEX_SHADER}
	}};

	ShaderInfo skyInfo;
	skyInfo.vertex = shaderFiles->sky.vertex;
	skyInfo.fragment = shaderFiles->sky.fragment;
	skyInfo.pass = RenderPass::OPAQUE;
	skyInfo.buffer = GENERIC_BUFFER;
	skyInfo.uniformSets = {SCREEN_SET, CUBE_SET};
	skyInfo.pushConstants = {{
		{UniformType::MAT4, "modelView", UniformProviderType::OBJECT_MODEL_VIEW, USE_VERTEX_SHADER}
	}};

	loader->loadShader(BASIC_SHADER, basicInfo);
	loader->loadShader(PHONG_SHADER, phongInfo);
	loader->loadShader(TEXT_SHADER, textInfo);
	loader->loadShader(SKYBOX_SHADER, skyInfo);
}

void Game::loadScreens(DisplayEngine& display) {
	//Run a "demo" version of the game in the background.
	std::shared_ptr<Screen> demoWorld = ArenaGenerator::generateArena(display, false, 500);
	display.pushScreen(demoWorld);

	//Create main menu.
	std::shared_ptr<Screen> mainMenu = std::make_shared<Screen>(display, false);
	mainMenu->setState(std::make_shared<EmptyScreenState>());

	//Add component managers.
	mainMenu->addComponentManager(std::make_shared<RenderComponentManager>());
	mainMenu->addComponentManager(std::make_shared<GuiComponentManager>());
	mainMenu->addComponentManager(std::make_shared<PhysicsComponentManager>());

	//Create buttons.
	std::shared_ptr<Object> quitButton = std::make_shared<Object>();
	std::shared_ptr<Object> startButton = std::make_shared<Object>();

	quitButton->setState(std::make_shared<ButtonState>(glm::vec3(0.9, 0.1, 0.0)));
	startButton->setState(std::make_shared<ButtonState>(glm::vec3(0.0, 1.0, 0.0)));

	quitButton->addComponent(std::make_shared<BackButton>(Key::ESCAPE));
	quitButton->addComponent(std::make_shared<RenderComponent>(BUTTON_MODEL));
	quitButton->addComponent(std::make_shared<PhysicsComponent>(std::make_shared<BoxPhysicsObject>(Engine::instance->getModel(SQUARE_MODEL)->getMesh().getBox(), glm::vec3(0.0, -0.6, 0.0), 0.0f)));

	startButton->addComponent(std::make_shared<StartButton>(Key::ENTER));
	startButton->addComponent(std::make_shared<RenderComponent>(BUTTON_MODEL));
	startButton->addComponent(std::make_shared<PhysicsComponent>(std::make_shared<BoxPhysicsObject>(Engine::instance->getModel(SQUARE_MODEL)->getMesh().getBox(), glm::vec3(0.0, 0.4, 0.0), 0.0f)));

	//Add a title thingy.
	std::shared_ptr<Object> title = std::make_shared<Object>();

	title->addComponent(std::make_shared<TextComponent>(U"Main Menu", FONT_TEX, TEXT_SHADER, TEXT_BUFFER, TEXT_SET));
	std::shared_ptr<TextComponent> menuText = title->getComponent<TextComponent>(TEXT_COMPONENT_NAME);
	menuText->fitToBox(glm::vec2(9.8, 9.8));

	AxisAlignedBB textBox = title->getComponent<TextComponent>(TEXT_COMPONENT_NAME)->getTextBox();
	//For position, doesn't take input.
	title->addComponent(std::make_shared<GuiComponent>(glm::vec3(textBox.min.x, 3.5, 0.0)));

	//Add buttons and title to menu.
	mainMenu->addObject(quitButton);
	mainMenu->addObject(startButton);
	mainMenu->addObject(title);

	//Set camera
	std::static_pointer_cast<DefaultCamera>(mainMenu->getCamera())->setPos(glm::vec3(0.0, 0.0, 10.0));

	display.pushOverlay(mainMenu);
}
