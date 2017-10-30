#pragma once

#include <string>
#include "RenderManager.hpp"

class Button {
public:
	Button(float centerX, float centerY, float width, float height, std::string textureName);

	virtual ~Button() {}

	virtual void onHoverStart() = 0;
	virtual void onHoverStop() = 0;
	virtual void onPress() = 0;
	virtual void onRelease() = 0;

	virtual void update() {}
	virtual void render(RenderManager& renderer, float partialTicks);

	virtual bool isWithin(float mouseX, float mouseY);

protected:
	float x;
	float y;
	float width;
	float height;
	std::string texture;
};
