#version 330 core
layout (location = 0) in uvec3 aPos;

uniform mat4 lightSpaceMatrix;

void main() {
    vec3 pos = vec3(aPos);
    
    gl_Position = lightSpaceMatrix * vec4(pos, 1.0);
}