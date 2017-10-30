#pragma once

#include "RenderManager.hpp"
#include "GameSettings.hpp"
#include "KeyTracker.hpp"
#include "ScreenManager.hpp"
#include "Screen.hpp"

class Game {
public:
	//The time taken for each tick, in milliseconds
	constexpr static double timeStep = 1000.0 / 60.0;

	Game();
	~Game();

	bool shouldExit() const;

	void update();
	void render(float partialTicks);
	void handleKeyboardInput(int key, int scanCode, int action);
	void handleMouseScroll(double offset);
	void handleMouseMove(double x, double y);
	void handleMouseClick(int button, int action);

	void setViewPort(int width, int height);

	void onExit();

private:
	GameSettings settings;
	KeyTracker keyTracker;
	RenderManager renderer;
	ScreenManager screenManager;
};
