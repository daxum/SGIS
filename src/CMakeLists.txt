cmake_minimum_required(VERSION 3.1)
project(SGIS)

set(ENGINE_DIR "" CACHE STRING "Engine repository directory")

if ("${ENGINE_DIR}" STREQUAL "")
	message(FATAL_ERROR "Engine directory not set")
endif()

add_subdirectory("${ENGINE_DIR}" ${CMAKE_CURRENT_BINARY_DIR}/engine)

add_executable(squareGame
	Main.cpp
	Game.cpp
	ControlledAI.cpp
	SquareSpawner.cpp
	SquareCamera.cpp
	StartButton.cpp
	ArenaGenerator.cpp
)

set_target_properties(squareGame PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED ON
)

if ((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
	target_compile_options(squareGame PRIVATE "-Wall")
endif()

target_link_libraries(squareGame Engine)

#Copy resources to build directory

add_custom_command(
	TARGET squareGame POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/../resources $<TARGET_FILE_DIR:squareGame>
)

