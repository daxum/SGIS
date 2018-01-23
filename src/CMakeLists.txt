cmake_minimum_required(VERSION 3.1)
project(SGIS)

add_subdirectory(../../engine/src ${CMAKE_CURRENT_BINARY_DIR}/engine)

add_executable(squareGame
	Main.cpp
	Game.cpp
)

set_target_properties(squareGame PROPERTIES
	CXX_STANDARD 11
	CXX_STANDARD_REQUIRED ON
)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	target_compile_options(squareGame PRIVATE "-Wall")
endif()

target_link_libraries(squareGame Engine)

add_custom_command(
	TARGET squareGame POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/../resources $<TARGET_FILE_DIR:squareGame>
)

