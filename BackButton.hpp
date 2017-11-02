#pragma once

#include <string>
#include "Button.hpp"
#include "ScreenManager.hpp"

class BackButton : public Button {
public:
	BackButton(ScreenManager& manager, float centerX, float centerY, float width, float height, std::string textureName);

	void onHoverStart();
	void onHoverStop();
	void onPress();
	void onRelease();

protected:
	ScreenManager& screenManager;
	std::string texName;
};
