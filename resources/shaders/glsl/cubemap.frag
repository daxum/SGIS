#version 410 core

in vec3 tex;

out vec4 outColor;

uniform samplerCube cubemap;

void main() {
	outColor = texture(cubemap, tex);
}

