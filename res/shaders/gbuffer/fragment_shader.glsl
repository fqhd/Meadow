#version 330 core

layout (location = 0) out vec4 FragPos;
layout (location = 1) out vec4 Normal;
layout (location = 2) out vec4 Albedo;

in vec3 position;
in vec3 color;
in vec3 normal;

void main() {
	FragPos = vec4(position, 1.0);
	Normal = vec4(normal, 1.0);
	Albedo = vec4(color, 1.0);
}