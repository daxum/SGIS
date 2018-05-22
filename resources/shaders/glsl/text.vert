#version 410 core

layout (location = 0) in vec2 posIn;
layout (location = 1) in vec2 texIn;

out vec2 tex;

uniform mat4 modelView;
uniform mat4 projection;

void main() {
	tex = texIn;
	gl_Position = projection * modelView * vec4(posIn, 0.0, 1.0);
}
