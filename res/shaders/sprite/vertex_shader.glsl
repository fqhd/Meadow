#version 330 core

out vec2 uv;

uniform vec4 destRect;
uniform vec4 uvRect;

uniform mat4 ortho;

void main() {
    vec2 position;
    if(gl_VertexID == 0) {
        position = vec2(destRect.x, destRect.y);
        uv = vec2(uvRect.x, uvRect.y);
    }else if(gl_VertexID == 1) {
        position = vec2(destRect.x, destRect.y + destRect.w);
        uv = vec2(uvRect.x, uvRect.y + uvRect.w);
    }else if(gl_VertexID == 2) {
        position = vec2(destRect.x + destRect.z, destRect.y);
        uv = vec2(uvRect.x + uvRect.z, uvRect.y);
    }else if(gl_VertexID == 3) {
        position = vec2(destRect.x + destRect.z, destRect.y + destRect.w);
        uv = vec2(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
    }

    gl_Position = ortho * vec4(position, 0.0, 1.0);
}
