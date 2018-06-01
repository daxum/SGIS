#version 410 core

layout (location = 0) in vec3 posIn;

//Currently ignored, needed because cube is in the static buffer
layout (location = 1) in vec3 normIn;
layout (location = 2) in vec2 texIn;

out vec3 tex;

uniform mat4 modelView;
uniform mat4 projection;

void main() {
	tex = posIn;
	gl_Position = (projection * modelView * vec4(posIn, 0.0)).xyww;
}

