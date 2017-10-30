cmake_minimum_required(VERSION 3.1)
project(squareGame)

find_package(glfw3 3.2 REQUIRED)

add_executable(squareGame
	main.cpp
	gl_core_4_1.c
	Shader.cpp
	Logger.cpp
	Game.cpp
	ShaderManager.cpp
	TextureManager.cpp
	Texture.cpp
	RenderManager.cpp
	arena.cpp
	KeyTracker.cpp
	ExtraMath.cpp
	ArenaRenderer.cpp
	ScreenManager.cpp
	Gui.cpp
	SpriteRenderer.cpp
	Button.cpp
	ScreenChangeButton.cpp
)

set_target_properties(squareGame PROPERTIES
	CXX_STANDARD 11
	CXX_STANDARD_REQUIRED ON
)

target_link_libraries(squareGame glfw GL)
