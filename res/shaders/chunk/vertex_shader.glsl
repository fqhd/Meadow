#version 330 core

layout (location = 0) in uvec3 position;
layout (location = 1) in uvec3 color;
layout (location = 2) in int n;

out vec3 fragColor;
out vec4 fragPosLightSpace;
out vec3 normal;
out vec3 lightDir;
out vec3 textureData;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;
uniform vec3 lightPos;

vec2 uvsF[6] = vec2[6](
    vec2(0.0f, 0.0f),
    vec2(0.0f, 1.0f),
    vec2(1.0f, 1.0f),
    vec2(0.0f, 0.0f),
	vec2(1.0f, 1.0f),
	vec2(1.0f, 0.0f)
);

vec2 uvsB[6] = vec2[6](
    vec2(0.0f, 0.0f),
	vec2(1.0f, 1.0f),
	vec2(0.0f, 1.0f),
    
	
	vec2(0.0f, 0.0f),
	vec2(1.0f, 0.0f),
	vec2(1.0f, 1.0f)
);


void main(){
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
	lightDir = normalize(lightPos - position);
	
	gl_Position = projection * view * vec4(position, 1.0);
	fragColor = vec3(color) / 255.0;
	fragPosLightSpace = lightSpaceMatrix * vec4(position, 1.0);
	
	if(n == 5 || n == 1 || n == 2) {
		textureData = vec3(uvsB[gl_VertexID%6], 2.0);
	}else{
		textureData = vec3(uvsF[gl_VertexID%6], 2.0);
	}
}
