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

#include "Map.hpp"
#include "FlatMapRenderData.hpp"

class FlatMap : public Map {
public:
	/**
	 * Creates a map of uniform height.
	 * @param size The x/z size of the map.
	 * @param model The model to use for rendering.
	 */
	 FlatMap(float size, std::string model) : renderData(std::make_shared<FlatMapRenderData>(size, model)), size(size) {}

	/**
	 * Returns the height at the given position.
	 */
	float getHeight(glm::vec3 pos) { return 0.0f; }

	/**
	 * Returns the render data for this map.
	 * @return A pointer to this map's rendering data.
	 */
	virtual std::shared_ptr<RenderData> getRenderData() { return renderData; }

private:
	std::shared_ptr<FlatMapRenderData> renderData;
	float size;
};
