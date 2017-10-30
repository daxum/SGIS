#pragma once

#include <unordered_set>

class KeyTracker {
public:
	bool isKeyPressed(int key) const;
	void setPressed(int key, bool pressed);

private:
	std::unordered_set<int> pressedKeys;
};
