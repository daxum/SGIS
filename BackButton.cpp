#include "BackButton.hpp"

using namespace std;

BackButton::BackButton(ScreenManager& manager, float x, float y, float width, float height, string texture) :
	Button(x, y, width, height, texture),
	screenManager(manager),
	texName(texture) {

}

void BackButton::onHoverStart() {
	texture = texName + "_hover";
}

void BackButton::onHoverStop() {
	texture = texName;
}

void BackButton::onPress() {
	texture = texName + "_press";
}

void BackButton::onRelease() {
	screenManager.popScreenStack();
	texture = texName;
}
