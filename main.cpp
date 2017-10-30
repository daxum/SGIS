#include <stdexcept>
#include "callbackList.h"
#include "ExtraMath.hpp"
#include "combinedGl.h"
#include "Logger.hpp"
#include "Game.hpp"

namespace {
	Game* squareGame = nullptr;
}

int main(int argc, char** argv) {
	glfwSetErrorCallback(errorCallback);
	Logger::pushDomain("LOAD");

	try {
		squareGame = new Game();
	}
	catch(const std::runtime_error& e) {
		Logger::fatal() << "Error initializing game: " << e.what() << "\n";
		glfwTerminate();

		return -1;
	}

	Logger::popDomain();
	Logger::pushDomain("RUN");

	double currentTime = ExMath::getTimeMillis();
	double lag = 0.0;

	while (!squareGame->shouldExit()) {
		glfwPollEvents();

		double newTime = ExMath::getTimeMillis();
		double frameTime = newTime - currentTime;

		if (frameTime > 100.0) {
			frameTime = 100.0;
		}

		currentTime = newTime;
		lag += frameTime;

		int loops = 0;
		while(lag >= Game::timeStep && loops < 10) {
			squareGame->update();
			lag -= Game::timeStep;
			loops++;
		}

		if (loops >= 10) {
			Logger::warn() << "Currently running " << lag << " milliseconds behind\n";
			lag = 0.0;
		}

		squareGame->render((float)lag / Game::timeStep);
	}

	Logger::popDomain();
	Logger::pushDomain("EXIT");

	squareGame->onExit();

	delete squareGame;

	Logger::popDomain();

	return 0;
}

//Callbacks - anything requiring an implementation of more than one line should probably be offloaded to Game

void errorCallback(int error, const char* description) {
	Logger::error() << "GLFW error code " << error << ": " << description << "\n";
}

void framebufferSizeChange(GLFWwindow* window, int width, int height) {
	squareGame->setViewPort(width, height);
}

void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods) {
	squareGame->handleKeyboardInput(key, scanCode, action);
}

void mouseScroll(GLFWwindow* window, double xOffset, double yOffset) {
	squareGame->handleMouseScroll(yOffset);
}

void cursorMove(GLFWwindow* window, double x, double y) {
	squareGame->handleMouseMove(x, y);
}

void mouseClick(GLFWwindow* window, int button, int action, int mods) {
	squareGame->handleMouseClick(button, action);
}
