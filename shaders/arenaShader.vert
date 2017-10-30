#version 410 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 inputTexCoords;

out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	texCoords = inputTexCoords;
	gl_Position = projection * view * model * vec4(position, 0.0, 1.0);
}
