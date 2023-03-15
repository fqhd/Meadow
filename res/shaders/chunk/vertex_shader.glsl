#version 330 core

layout (location = 0) in uvec3 position;
layout (location = 1) in uvec3 color;

out vec3 fragColor;

uniform mat4 view;
uniform mat4 projection;

void main(){
	gl_Position = projection * view * vec4(position, 1.0);
	fragColor = color / 255.0;
}
