/******************************************************************************
 * SGIS - A simple game involving squares
 * Copyright (C) 2017
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#include <stdexcept>
#include "ScreenManager.hpp"
#include "Screen.hpp"
#include "combinedGl.h"

using namespace std;

ScreenManager::~ScreenManager() {
	for (const pair<string, Screen*>& element : screens) {
		delete element.second;
	}

	screens.clear();
}

void ScreenManager::addScreen(string name, Screen* screen) {
	if (screens.count(name) == 0) {
		screens.insert(make_pair(name, screen));
	}
	else {
		delete screen;
		throw invalid_argument("Duplicate screen name \"" + name + "\"");
	}
}

void ScreenManager::openScreen(string name) {
	Screen* toDisplay = screens.at(name);

	for (const Screen* screen : activeScreenStack.top()) {
		if (screen == toDisplay) {
			throw invalid_argument("Attempted to open already open screen \"" + name + "\"");
		}
	}

	activeScreenStack.top().push_back(toDisplay);
	toDisplay->onOpen();
}

void ScreenManager::closeScreen(string name) {
	Screen* toClose = screens.at(name);
	vector<Screen*>& activeScreens = activeScreenStack.top();

	for (size_t i = 0; i < activeScreens.size(); i++) {
		if (activeScreens.at(i) == toClose) {
			activeScreens.erase(activeScreens.begin() + i);
			toClose->onClose();
			return;
		}
	}

	throw invalid_argument("Attempted to close non-open screen \"" + name + "\"");
}

void ScreenManager::pushScreenStack() {
	activeScreenStack.emplace();
}

void ScreenManager::popScreenStack() {
	vector<Screen*> oldTop = activeScreenStack.top();

	activeScreenStack.pop();

	for (Screen* s : oldTop) {
		s->onClose();
	}
}

void ScreenManager::update() {
	if (!activeScreenStack.empty()) {
		for (Screen* screen : activeScreenStack.top()) {
			screen->update();
		}
	}
}

void ScreenManager::render(RenderManager& renderer, float partialTicks) {
	if (!activeScreenStack.empty()) {
		for (Screen* screen : activeScreenStack.top()) {
			glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			screen->render(renderer, partialTicks);
		}
	}
}

void ScreenManager::onKeyPress(int key) {
	vector<Screen*>::reverse_iterator rit = activeScreenStack.top().rbegin();
	vector<Screen*>::reverse_iterator rend = activeScreenStack.top().rend();

	for (;rit != rend; rit++) {
		if ((*rit)->onKeyPress(key)) {
			break;
		}
	}
}

void ScreenManager::onMouseScroll(double offset) {
	vector<Screen*>::reverse_iterator rit = activeScreenStack.top().rbegin();
	vector<Screen*>::reverse_iterator rend = activeScreenStack.top().rend();

	for (;rit != rend; rit++) {
		if ((*rit)->onMouseScroll(offset)) {
			break;
		}
	}
}

void ScreenManager::onMouseMove(float x, float y) {
	vector<Screen*>::reverse_iterator rit = activeScreenStack.top().rbegin();
	vector<Screen*>::reverse_iterator rend = activeScreenStack.top().rend();

	for (;rit != rend; rit++) {
		if ((*rit)->onMouseMove(x, y)) {
			break;
		}
	}
}

void ScreenManager::onMouseClick(int button, int action) {
	vector<Screen*>::reverse_iterator rit = activeScreenStack.top().rbegin();
	vector<Screen*>::reverse_iterator rend = activeScreenStack.top().rend();

	for (;rit != rend; rit++) {
		if ((*rit)->onMouseClick(button, action)) {
			break;
		}
	}
}

bool ScreenManager::shouldExit() const {
	return activeScreenStack.empty();
}
