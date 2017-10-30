#version 410 core

in vec2 texCoords;

out vec4 outColor;

uniform sampler2D texture0;
uniform vec3 color;

void main() {
	outColor = texture(texture0, texCoords) * vec4(color, 1.0);

	if (outColor.w < 0.5) {
		discard;
	}
}
