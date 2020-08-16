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

#include <glm/glm.hpp>

#include "Display/Camera.hpp"
#include "Engine.hpp"
#include "Display/WindowSizeEvent.hpp"
#include "Display/ScreenChangeEvent.hpp"

class GuiCamera : public Camera {
public:
	GuiCamera() : near(1.0), far(100.0) {}

	const glm::mat4 getView() const override { return glm::lookAt(glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0)); }
	const glm::mat4 getProjection() const override { return projection; }

	bool onEvent(const std::shared_ptr<const Event> event) override {
		if (event->type == WindowSizeEvent::EVENT_TYPE || event->type == ScreenChangeEvent::EVENT_TYPE) {
			projection = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, near, far);
		}

		return false;
	}

	std::pair<float, float> getNearFar() const override { return {near, far}; }

	float getFOV() const override { return 0.0f; }

	void update() override {}

private:
	//Near plane, far plane, and projection matrix.
	float near;
	float far;
	glm::mat4 projection;
};
