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

#version 430 core

layout (location = 0) in vec3 posIn;
//Currently ignored, needed because cube is in the static buffer
layout (location = 1) in vec3 normIn;
layout (location = 2) in vec2 texIn;

out vec3 tex;

//Screen set
layout(binding = 0, std140) uniform ScreenData {
	mat4 projection;
	mat4 view;
	vec3 light;
} screen;

//Object set
uniform mat4 modelView;

void main() {
	tex = posIn;
	gl_Position = (screen.projection * modelView * vec4(posIn, 0.0)).xyww;
}

