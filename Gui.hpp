#pragma once

#include <string>
#include <vector>
#include "Screen.hpp"
#include "Button.hpp"
#include "RenderManager.hpp"

class Gui : public Screen {
public:
	Gui(ScreenManager& manager, GameSettings& settings, std::string screenName);
	~Gui();

	void onOpen();
	void onClose();

	void update();
	void render(RenderManager& renderer, float partialTicks);

	bool onMouseMove(float x, float y);
	bool onMouseClick(int button, int action);

	void setBackground(std::string textureName);
	void setPopOnClose(bool shouldPop);

	void addButton(Button* button);

private:
	std::vector<Button*> buttons;
	Button* activeButton;
	Button* mouseDownButton;

	bool popOnClose;
	bool hasBackground;
	std::string background;
};
