#include "Button.hpp"

using namespace std;

Button::Button(float centerX, float centerY, float width, float height, string textureName) :
	x(centerX),
	y(centerY),
	width(width),
	height(height),
	texture(textureName) {

}

void Button::render(RenderManager& renderer, float partialTicks) {
	renderer.getSpriteRenderer()->addSprite(x, y, width, height, texture);
}

bool Button::isWithin(float mouseX, float mouseY) {
	return mouseX >= (x - width / 2.0f) && mouseX <= (x + width / 2.0f) &&
		   mouseY >= (y - height / 2.0f) && mouseY <= (y + height / 2.0f);
}
