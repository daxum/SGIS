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

#pragma once

#include "combinedGl.h"

/*
 * This file lists all the callback functions to be registered with glfw.
 * It is only used to register callbacks, and all implementations are in main.
 */

void errorCallback(int error, const char* description);

void framebufferSizeChange(GLFWwindow* window, int width, int height);

void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);

void mouseScroll(GLFWwindow* window, double xOffset, double yOffset);

void cursorMove(GLFWwindow* window, double x, double y);

void mouseClick(GLFWwindow* window, int button, int action, int mods);
