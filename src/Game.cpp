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
#include "Models/VertexFormat.hpp"
#include "Names.hpp"
#include "SquareWorldState.hpp"
#include "Models/ModelLoader.hpp"

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

void Game::createRenderObjects(RenderInitializer& renderInit) {
	renderInit.createBuffer(GENERIC_VERTEX_BUFFER, 1048576, BufferType::VERTEX, BufferStorage::DEVICE);
	renderInit.createBuffer(GENERIC_INDEX_BUFFER, 1048576, BufferType::INDEX, BufferStorage::DEVICE);
	renderInit.createBuffer(TEXT_VERTEX_BUFFER, 1048576, BufferType::VERTEX, BufferStorage::DEVICE);
	renderInit.createBuffer(TEXT_INDEX_BUFFER, 1048576, BufferType::INDEX, BufferStorage::DEVICE);

	renderInit.addVertexFormat(GENERIC_FORMAT, VertexFormat({
		{VERTEX_ELEMENT_POSITION, VertexFormat::ElementType::VEC3},
		{VERTEX_ELEMENT_NORMAL, VertexFormat::ElementType::VEC3},
		{VERTEX_ELEMENT_TEXTURE, VertexFormat::ElementType::VEC2}
	}));

	renderInit.addVertexFormat(TEXT_FORMAT, VertexFormat({
		{VERTEX_ELEMENT_POSITION, VertexFormat::ElementType::VEC2},
		{VERTEX_ELEMENT_TEXTURE, VertexFormat::ElementType::VEC2}
	}));

	renderInit.addUniformSet(PHONG_SET, UniformSetType::MATERIAL, 3,
		{{UniformType::SAMPLER_2D, UNIFORM_NAME_KD_TEX, UniformProviderType::MATERIAL, USE_FRAGMENT_SHADER},
		{UniformType::VEC3, UNIFORM_NAME_KA, UniformProviderType::MATERIAL, USE_FRAGMENT_SHADER},
		{UniformType::VEC3, UNIFORM_NAME_KS, UniformProviderType::MATERIAL, USE_FRAGMENT_SHADER},
		{UniformType::FLOAT, UNIFORM_NAME_SHININESS, UniformProviderType::MATERIAL, USE_FRAGMENT_SHADER}}
	);

	renderInit.addUniformSet(BASIC_SET, UniformSetType::MATERIAL, 1,
		{{UniformType::SAMPLER_2D, UNIFORM_NAME_KA_TEX, UniformProviderType::MATERIAL, USE_FRAGMENT_SHADER}}
	);

	renderInit.addUniformSet(CUBE_SET, UniformSetType::MATERIAL, 1,
		{{UniformType::SAMPLER_CUBE, UNIFORM_NAME_KA_TEX, UniformProviderType::MATERIAL, USE_FRAGMENT_SHADER}}
	);

	renderInit.addUniformSet(TEXT_SET, UniformSetType::MATERIAL, 1,
		{{UniformType::SAMPLER_2D, UNIFORM_NAME_KA_TEX, UniformProviderType::MATERIAL, USE_FRAGMENT_SHADER}}
	);

	renderInit.addUniformSet(SCREEN_SET, UniformSetType::PER_SCREEN, 3,
		{{UniformType::MAT4, "projection", UniformProviderType::CAMERA_PROJECTION, USE_VERTEX_SHADER},
		{UniformType::MAT4, "view", UniformProviderType::CAMERA_VIEW, USE_VERTEX_SHADER},
		{UniformType::VEC3, "light", UniformProviderType::SCREEN_STATE, USE_VERTEX_SHADER}}
	);
}

void Game::loadTextures(std::shared_ptr<TextureLoader> loader) {
	loader->loadTexture(SQUARE_TEX, "textures/square.png", Filter::NEAREST, Filter::NEAREST, true);
	loader->loadTexture(ARENA_TEX, "textures/arena.png", Filter::LINEAR, Filter::LINEAR, true);
	loader->loadTexture(WALL_TEX, "textures/wall.png", Filter::LINEAR, Filter::LINEAR, true);
	loader->loadFont(FONT_TEX, {"fonts/DejaVuSans.ttf"}, U"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789:;'\".,-!?", 191);
	loader->loadCubeMap(SKY_TEX, {"textures/sky_east.png", "textures/sky_west.png", "textures/sky_north.png", "textures/sky_north.png", "textures/sky_south.png", "textures/sky_north.png"}, Filter::LINEAR, Filter::LINEAR, true);
}

void Game::loadModels(ModelLoader& loader) {
	MaterialCreateInfo squareMatInfo = {
		.filename = "models/square.mtl",
		.shader = PHONG_SHADER,
		.uniformSet = PHONG_SET,
		.viewCull = true,
	};

	MaterialCreateInfo buttonMatInfo = {
		.filename = "models/square.mtl",
		.shader = BASIC_SHADER,
		.uniformSet = BASIC_SET,
		.viewCull = true,
	};

	MaterialCreateInfo arenaMatInfo = {
		.filename = "models/arena.mtl",
		.shader = PHONG_SHADER,
		.uniformSet = PHONG_SET,
		.viewCull = true,
	};

	MaterialCreateInfo wallMatInfo = {
		.filename = "models/wall.mtl",
		.shader = PHONG_SHADER,
		.uniformSet = PHONG_SET,
		.viewCull = true,
	};

	MaterialCreateInfo skyMatInfo = {
		.filename = "models/sky.mtl",
		.shader = SKYBOX_SHADER,
		.uniformSet = CUBE_SET,
		.viewCull = false,
	};

	loader.loadMaterial(SQUARE_MAT, squareMatInfo);
	loader.loadMaterial(BUTTON_MAT, buttonMatInfo);
	loader.loadMaterial(ARENA_MAT, arenaMatInfo);
	loader.loadMaterial(WALL_MAT, wallMatInfo);
	loader.loadMaterial(SKY_MAT, skyMatInfo);

	Material textMat(TEXT_MAT, TEXT_SHADER, TEXT_SET, Engine::instance->getModelManager().getMemoryManager()->getUniformSet(TEXT_SET));
	Engine::instance->getModelManager().addMaterial(TEXT_MAT, std::move(textMat));

	MeshCreateInfo squareMeshInfo = {
		.filename = "models/square.obj",
		.vertexBuffer = GENERIC_VERTEX_BUFFER,
		.indexBuffer = GENERIC_INDEX_BUFFER,
		.vertexFormat = GENERIC_FORMAT,
		.renderable = true,
	};

	MeshCreateInfo arenaMeshInfo = {
		.filename = "models/arena.obj",
		.vertexBuffer = GENERIC_VERTEX_BUFFER,
		.indexBuffer = GENERIC_INDEX_BUFFER,
		.vertexFormat = GENERIC_FORMAT,
		.renderable = true,
	};

	MeshCreateInfo wallMeshInfo = {
		.filename = "models/cube.obj",
		.vertexBuffer = GENERIC_VERTEX_BUFFER,
		.indexBuffer = GENERIC_INDEX_BUFFER,
		.vertexFormat = GENERIC_FORMAT,
		.renderable = true,
	};

	MeshCreateInfo skyMeshInfo = {
		.filename = "models/cube_cw.obj",
		.vertexBuffer = GENERIC_VERTEX_BUFFER,
		.indexBuffer = GENERIC_INDEX_BUFFER,
		.vertexFormat = GENERIC_FORMAT,
		.renderable = true,
	};

	loader.loadMesh(SQUARE_MESH, squareMeshInfo);
	loader.loadMesh(ARENA_MESH, arenaMeshInfo);
	loader.loadMesh(WALL_MESH, wallMeshInfo);
	loader.loadMesh(SKY_MESH, skyMeshInfo);
}

void Game::loadShaders(std::shared_ptr<ShaderLoader> loader) {
	ShaderInfo basicInfo;
	basicInfo.vertex = shaderFiles->basic.vertex;
	basicInfo.fragment = shaderFiles->basic.fragment;
	basicInfo.pass = RenderPass::OPAQUE;
	basicInfo.format = GENERIC_FORMAT;
	basicInfo.uniformSets = {SCREEN_SET, BASIC_SET};
	basicInfo.pushConstants = {{
		{UniformType::MAT4, "modelView", UniformProviderType::OBJECT_MODEL_VIEW, USE_VERTEX_SHADER},
		{UniformType::VEC3, "color", UniformProviderType::OBJECT_STATE, USE_FRAGMENT_SHADER}
	}};

	ShaderInfo phongInfo;
	phongInfo.vertex = shaderFiles->phong.vertex;
	phongInfo.fragment = shaderFiles->phong.fragment;
	phongInfo.pass = RenderPass::OPAQUE;
	phongInfo.format = GENERIC_FORMAT;
	phongInfo.uniformSets = {SCREEN_SET, PHONG_SET};
	phongInfo.pushConstants = {{
		{UniformType::MAT4, "modelView", UniformProviderType::OBJECT_MODEL_VIEW, USE_VERTEX_SHADER},
		{UniformType::VEC3, "color", UniformProviderType::OBJECT_STATE, USE_FRAGMENT_SHADER}
	}};

	ShaderInfo textInfo;
	textInfo.vertex = shaderFiles->text.vertex;
	textInfo.fragment = shaderFiles->text.fragment;
	textInfo.pass = RenderPass::TRANSLUCENT;
	textInfo.format = TEXT_FORMAT;
	textInfo.uniformSets = {SCREEN_SET, TEXT_SET};
	textInfo.pushConstants = {{
		{UniformType::MAT4, "modelView", UniformProviderType::OBJECT_MODEL_VIEW, USE_VERTEX_SHADER}
	}};

	ShaderInfo skyInfo;
	skyInfo.vertex = shaderFiles->sky.vertex;
	skyInfo.fragment = shaderFiles->sky.fragment;
	skyInfo.pass = RenderPass::OPAQUE;
	skyInfo.format = GENERIC_FORMAT;
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
	quitButton->addComponent(std::make_shared<RenderComponent>(BUTTON_MAT, SQUARE_MESH));

	PhysicsInfo buttonInfo = {
		.shape = PhysicsShape::BOX,
		.box = Engine::instance->getModelManager().getMesh(SQUARE_MESH, CacheLevel::MEMORY)->getMesh()->getBox(),
		.pos = glm::vec3(0.0, -0.6, 0.0),
		.mass = 0.0f,
	};

	quitButton->addComponent(std::make_shared<PhysicsComponent>(std::make_shared<PhysicsObject>(buttonInfo)));

	startButton->addComponent(std::make_shared<StartButton>(Key::ENTER));
	startButton->addComponent(std::make_shared<RenderComponent>(BUTTON_MAT, SQUARE_MESH));

	buttonInfo.pos = glm::vec3(0.0, 0.4, 0.0);

	startButton->addComponent(std::make_shared<PhysicsComponent>(std::make_shared<PhysicsObject>(buttonInfo)));

	//Add a title thingy.
	TextMeshInfo textInfo = {
		.font = FONT_TEX,
		.text = U"Main Menu",
		.vertexBuffer = TEXT_VERTEX_BUFFER,
		.indexBuffer = TEXT_INDEX_BUFFER,
		.format = TEXT_FORMAT,
	};

	std::shared_ptr<Object> title = std::make_shared<Object>();

	title->addComponent(std::make_shared<TextComponent>(textInfo, TEXT_MAT));
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
