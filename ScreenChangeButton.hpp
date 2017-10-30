#pragma once

#include "Button.hpp"
#include "ScreenManager.hpp"

class ScreenChangeButton : public Button {
public:
	ScreenChangeButton(ScreenManager& manager, float x, float y, float width, float height, std::string screen, std::string texture);

	void onHoverStart();
	void onHoverStop();
	void onPress();
	void onRelease();

private:
	ScreenManager& screenManager;
	std::string texName;
	std::string screenName;
};
