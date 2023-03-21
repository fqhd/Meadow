#version 330 core

out vec2 uv;

uniform vec4 destRect;
uniform vec4 uvRect;
uniform mat4 ortho;

void main() {
    destRect
    uv = (aPosition + 1.0) / 2.0;
    gl_Position = vec4(aPosition, 0.0, 1.0);
}
