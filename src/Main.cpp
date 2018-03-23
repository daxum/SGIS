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
#include <iostream>
#include "Engine.hpp"
#include "Game.hpp"

int main(int argc, char** argv) {
	//Create instance of game engine
	EngineConfig config = {};
	config.renderer = Renderer::OPEN_GL;
	config.windowWidth = 960;
	config.windowHeight = 540;
	config.windowTitle = "Squares";
	config.timestep = 1000.0 / 60.0;

	config.generalLog.type = LogType::STDOUT;
	config.generalLog.mask = DEBUG | INFO | WARN | ERROR | FATAL;

	config.rendererLog.type = LogType::STDOUT;
	config.rendererLog.mask = DEBUG | INFO | WARN | ERROR | FATAL;

	config.loaderLog.type = LogType::STDOUT;
	config.loaderLog.mask = DEBUG | INFO | WARN | ERROR | FATAL;

	try {
		Engine engine(config);

		//Run game
		Game squareGame;
		engine.run(squareGame);
	}
	catch(const std::runtime_error& e) {
		std::cout << e.what() << "\n";
	}

	return 0;
}
