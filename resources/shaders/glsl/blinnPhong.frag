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

in vec3 pos;
in vec3 norm;
in vec2 tex;
in vec3 lightDir;

out vec4 outColor;

//Model set
uniform sampler2D diffuseTex;

layout(binding = 1, std140) uniform MaterialData {
	vec3 ka;
	vec3 ks;
	float s;
} material;

//Object set
uniform vec3 color;

vec3 directionalLight(float intensity) {
	vec3 texColor = color * texture(diffuseTex, tex).xyz;

	vec3 normal = normalize(norm);
	vec3 position = -normalize(pos);

	vec3 ambient = texColor * material.ka;
	vec3 diffuse = texColor * max(0, dot(normalize(lightDir), normal));
	vec3 specular = texColor * material.ks * pow(max(0, dot(normalize(normalize(lightDir) + position), normal)), material.s);

	return intensity * (ambient + diffuse + specular);
}

vec3 pointLight(vec3 lightPos) {
	vec3 texColor = color * texture(diffuseTex, tex).xyz;

	vec3 normal = normalize(norm);
	vec3 position = -normalize(pos);

	vec3 light = normalize(lightPos - pos);

	vec3 ambient = texColor * material.ka;
	vec3 diffuse = texColor * max(0, dot(light, normal));
	vec3 specular = texColor * material.ks * pow(max(0, dot(normalize(light + position), normal)), material.s);

	return ambient + diffuse + specular;
}

vec3 cel(vec3 color, float factor) {
	float average = (color.x + color.y + color.z) / 3.0;
	float adjAverage = ((ceil(average * factor) / factor) + (floor(average * factor) / factor)) / 2.0;
	color.x = color.x * (adjAverage / average);
	color.y = color.y * (adjAverage / average);
	color.z = color.z * (adjAverage / average);

	return color;
}

void main() {
	outColor = vec4(directionalLight(1.0), 1.0);

	//Comment/uncomment to toggle cel shading
	//outColor = vec4(cel(outColor.xyz, 20.0), outColor.w);
}
