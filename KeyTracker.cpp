#include "KeyTracker.hpp"

bool KeyTracker::isKeyPressed(int key) const {
	return pressedKeys.count(key) == 1;
}

void KeyTracker::setPressed(int key, bool pressed) {
	if (pressed) {
		pressedKeys.insert(key);
	}
	else {
		pressedKeys.erase(key);
	}
}
