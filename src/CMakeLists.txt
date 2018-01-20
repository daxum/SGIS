cmake_minimum_required(VERSION 3.1)
project(SGIS)

find_package(glfw3 3.2 REQUIRED)
find_package(assimp REQUIRED)

add_subdirectory(../../engine/src ${CMAKE_CURRENT_BINARY_DIR}/engine)

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
	BackButton.cpp
	Mesh.cpp
	Model.cpp
	ModelManager.cpp
	AxisAlignedBB.cpp
)

set_target_properties(squareGame PROPERTIES
	CXX_STANDARD 11
	CXX_STANDARD_REQUIRED ON
)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	target_compile_options(squareGame PRIVATE "-Wall")
endif()

target_link_libraries(squareGame glfw GL assimp)

add_custom_command(
	TARGET squareGame POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/../resources $<TARGET_FILE_DIR:squareGame>
)

