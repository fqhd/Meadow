#version 330 core

layout (location = 0) in uvec3 aPosition;
layout (location = 1) in uvec3 aColor;
layout (location = 2) in int n;

out vec3 position;
out vec3 normal;
out vec3 color;

uniform mat4 projection;
uniform mat4 view;

void main() {
	if(n == 0) {
		normal = vec3(0.0, 1.0, 0.0);
	}else if(n == 1) {
		normal = vec3(0.0, -1.0, 0.0);
	}else if(n == 2) {
		normal = vec3(-1.0, 0.0, 0.0);
	}else if(n == 3) {
		normal = vec3(1.0, 0.0, 0.0);
	}else if(n == 4) {
		normal = vec3(0.0, 0.0, -1.0);
	}else if(n == 5) {
		normal = vec3(0.0, 0.0, 1.0);
	}
	color = vec3(aColor) / 255.0;
	
	vec4 viewPos = view * vec4(aPosition, 1.0);
	
	normal = (view * vec4(normal, 0.0)).xyz;
	
	position = viewPos.xyz;

	gl_Position = projection * viewPos;
}
