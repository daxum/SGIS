#version 410 core

in vec2 tex;

out vec4 outColor;

uniform sampler2D tex0;
uniform vec3 color;

void main() {
	outColor = vec4(0.0, 0.0, 0.0, texture(tex0, tex).r);
}
