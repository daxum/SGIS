cmake_minimum_required(VERSION 3.1)
project(SGIS)

find_package(glfw3 3.2 REQUIRED)
find_package(assimp REQUIRED)

add_subdirectory(src)
