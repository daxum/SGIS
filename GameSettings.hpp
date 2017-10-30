#pragma once

//Platform-specific default keybindings
enum X11Keys {
	KEY_ESC = 9,
	KEY_W = 25,
	KEY_P = 33,
	KEY_A = 38,
	KEY_S = 39,
	KEY_D = 40
};

class GameSettings {
public:
	//Controls
	int keyMoveUp = KEY_W;
	int keyMoveDown = KEY_S;
	int keyMoveRight = KEY_D;
	int keyMoveLeft = KEY_A;

	//Menu shortcuts
	int keyPlay = KEY_P;
	int keyClose = KEY_ESC;

	//Arena settings - cannot change in game
	float sizeX = 1000.0f;
	float sizeY = 1000.0f;

	GameSettings() {}

private:
};
