#version 330 core

in vec2 uv;
out vec4 outColor;

void main() {
    outColor = vec4(uv, uv.x - uv.y, 1.0);
}