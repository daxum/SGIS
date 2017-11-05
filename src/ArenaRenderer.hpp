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

#include "combinedGl.h"
#include "Shader.hpp"
#include "RenderManager.hpp"

struct Square;
class Arena;

class ArenaRenderer {
public:
	constexpr static float MIN_CAMERA_HEIGHT = 5.0f;
	constexpr static float MAX_CAMERA_HEIGHT = 300.0f;
	constexpr static float MAX_CAMERA_VELOCITY = 5.0f;

	ArenaRenderer(Arena* arena);
	~ArenaRenderer();

	void updateCamera();
	void increaseCameraVelocity(float amount);
	void render(RenderManager& renderer, float partialTicks);

	float getCameraHeight();

private:
	Arena* arena;

	float prevCameraHeight;
	float cameraHeight;
	float cameraVelocity;

	GLuint squareVbo;
	GLuint borderEbo;
	GLuint squareVao;

	void renderSquare(RenderManager& renderer, const Shader* shader, const Square& toAdd, float partialTicks);
	void initGraphics();
};
