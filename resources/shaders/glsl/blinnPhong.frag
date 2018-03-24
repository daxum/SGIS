#version 410 core

in vec3 pos;
in vec3 norm;
in vec2 tex;

out vec4 outColor;

uniform sampler2D tex0;
uniform vec3 color;
uniform vec3 lightDir;

uniform vec3 ka;
uniform vec3 ks;
uniform float s;

vec3 directionalLight(float intensity) {
	vec3 normal = normalize(norm);

	vec3 diffuse = texture(tex0, tex).xyz * max(0, dot(lightDir, normal));
	vec3 specular = ks * pow(max(0, dot(normalize(lightDir + lightDir), normal)), s);

	return intensity * color * (ka + diffuse + specular);
}

vec3 pointLight(vec3 lightPos) {
	vec3 normal = normalize(norm);
	vec3 position = -normalize(pos);

	vec3 light = normalize(lightPos - pos);

	vec3 diffuse = texture(tex0, tex).xyz * max(0, dot(light, normal));
	vec3 specular = ks * pow(max(0, dot(normalize(light + position), normal)), s);

	return color * (ka + diffuse + specular);
}

void main() {
	outColor = vec4(directionalLight(1.0), 1.0);

	//Comment/uncomment to toggle cel shading
	//outColor = ceil(outColor * 20.0) / 20.0;
}
