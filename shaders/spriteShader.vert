#version 410 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex;

out vec2 texCoords;

uniform vec2 translation;
uniform vec2 scale;

void main() {
	texCoords = tex;
	gl_Position = vec4((pos + translation) * scale, 0.0, 1.0);
}
