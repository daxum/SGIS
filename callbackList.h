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
