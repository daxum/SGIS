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

#pragma once

#include "GameInterface.hpp"

class Game : public GameInterface {
public:
	static constexpr bool USE_VULKAN = false;

	/**
	 * Bleh.
	 * @param renderInit buffer / uniform set loader.
	 */
	void createRenderObjects(RenderInitializer& renderInit);

	/**
	 * Called from the engine, loads the game's textures.
	 * @param loader The texture loader provided by the engine.
	 */
	void loadTextures(std::shared_ptr<TextureLoader> loader);

	/**
	 * Called from the engine to load the game's models
	 * @param loader The model loader provided by the engine.
	 */
	void loadModels(ModelLoader& loader);

	/**
	 * Called from the engine to load the game's shaders.
	 * @param loader The shader loader provided by the engine.
	 */
	void loadShaders(std::shared_ptr<ShaderLoader> loader);

	/**
	 * Loads the main menu onto the display.
	 * @param display The display to add the menu to.
	 */
	void loadScreens(DisplayEngine& display);
};
