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

#include "RenderData.hpp"

class FlatMapRenderData : public RenderData {
public:
	/**
	 * Creates rendering data for the map.
	 * @param size The size of the map, used for scaling
	 * @param model The model for the map. Should probably be flat for things to make sense.
	 */
	 FlatMapRenderData(float size, std::string model) : scale(size, 1.0f, size), modelName(model) {}

	/**
	 * Returns the translation.
	 * @return A translation vector.
	 */
	glm::vec3 getTranslation() { return glm::vec3(0.0f, 0.0f, 0.0f); }

	/**
	 * Returns the rotation.
	 * @return A rotation vector.
	 */
	virtual glm::vec3 getRotation() { return glm::vec3(0.0f, 0.0f, 0.0f); }

	/**
	 * Returns the scale.
	 * @return The scale.
	 */
	virtual glm::vec3 getScale() { return scale; }

	/**
	 * Returns the name of the model to be used in rendering.
	 * @return The model name.
	 */
	virtual std::string getModel() { return modelName; }

private:
	glm::vec3 scale;
	std::string modelName;
};
