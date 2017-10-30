#include "ScreenChangeButton.hpp"

using namespace std;

ScreenChangeButton::ScreenChangeButton(ScreenManager& manager, float x, float y, float width, float height, string screen, string texture) :
	Button(x, y, width, height, texture),
	screenManager(manager),
	texName(texture),
	screenName(screen) {

}

void ScreenChangeButton::onHoverStart() {
	texture = texName + "_hover";
}

void ScreenChangeButton::onHoverStop() {
	texture = texName;
}

void ScreenChangeButton::onPress() {
	texture = texName + "_press";
}

void ScreenChangeButton::onRelease() {
	screenManager.pushScreenStack();
	screenManager.openScreen(screenName);
}
