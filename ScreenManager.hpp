#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <stack>
#include "RenderManager.hpp"

class Screen;

class ScreenManager {
public:
	~ScreenManager();

	void addScreen(std::string name, Screen* screen);

	void openScreen(std::string name);
	void closeScreen(std::string name);

	void pushScreenStack();
	void popScreenStack();

	void update();
	void render(RenderManager& renderer, float partialTicks);

	void onKeyPress(int key);
	void onMouseScroll(double offset);
	void onMouseMove(float x, float y);
	void onMouseClick(int button, int action);

	bool shouldExit() const;

private:
	std::unordered_map<std::string, Screen*> screens;
	std::stack<std::vector<Screen*>> activeScreenStack;
};
