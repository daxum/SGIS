#pragma once

#include <vector>
#include <string>
#include "ScreenManager.hpp"
#include "GameSettings.hpp"
#include "RenderManager.hpp"

class Screen {
public:
	Screen(ScreenManager& manager, GameSettings& settings, std::string screenName) :
		name(screenName), screenManager(manager), settings(settings) {}

	virtual ~Screen() {}

	virtual void onOpen() = 0;
	virtual void onClose() = 0;

	virtual void update() = 0;
	virtual void render(RenderManager& renderer, float partialTicks) = 0;

	virtual bool onKeyPress(int key) { return false; }
	virtual bool onMouseScroll(double offset) { return false; }
	virtual bool onMouseMove(float x, float y) { return false; }
	virtual bool onMouseClick(int button, int action) { return false; }

protected:
	const std::string name;
	ScreenManager& screenManager;
	GameSettings& settings;
};
