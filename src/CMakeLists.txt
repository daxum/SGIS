cmake_minimum_required(VERSION 3.1)
project(SGIS)

#Note: To build this, the engine repository must be within the same directory as this repository.
#That means the directory structure should look like this:
#
#    parent directory --> game repository
#           |
#           +-----------> engine repository (folder currently needs to be named "engine")
#
#Hopefully I can think of a better way to handle this later.

add_subdirectory(../../engine ${CMAKE_CURRENT_BINARY_DIR}/engine)

add_executable(squareGame
	Main.cpp
	Game.cpp
	ControlledAI.cpp
	SquareSpawner.cpp
	SquareCamera.cpp
	StartButton.cpp
)

set_target_properties(squareGame PROPERTIES
	CXX_STANDARD 11
	CXX_STANDARD_REQUIRED ON
)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	target_compile_options(squareGame PRIVATE "-Wall")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fopenmp")
endif()

target_link_libraries(squareGame Engine)

#Copy resources to build directory

add_custom_command(
	TARGET squareGame POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/../resources $<TARGET_FILE_DIR:squareGame>
)

