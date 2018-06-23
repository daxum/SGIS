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
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.hpp"
#include "Engine.hpp"
#include "ExtraMath.hpp"
#include "SplineAnimation.hpp"

class AnimatedCamera : public Camera {
public:
	AnimatedCamera(const std::vector<std::pair<glm::vec3, glm::quat>>& frames, float time) :
		anim(frames, time),
		currentTime(0),
		near(0.1f),
		far(6000.0f) {

	}

	glm::mat4 getView() {
		const std::pair<glm::vec3, glm::quat> loc = anim.getLocation((float)currentTime);
		return glm::lookAt(loc.first, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	}

	glm::mat4 getProjection() {
		return projection;
	}

	void setProjection() {
		float width = Engine::instance->getWindowInterface().getWindowWidth();
		float height = Engine::instance->getWindowInterface().getWindowHeight();

		projection = glm::perspective(ExMath::PI / 4.0f, width / height, near, far);
	}

	std::pair<float, float> getNearFar() {
		return {near, far};
	}

	void update() {
		currentTime++;
	}

private:
	//The animation to follow
	SplineAnimation anim;
	//number of ticks for the camera
	size_t currentTime;

	//Near plane, far plane, projection matrix.
	float near;
	float far;
	glm::mat4 projection;
};
