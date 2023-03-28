#version 330 core

out vec2 uv;

void main() {
    vec2 position;
    if(gl_VertexID == 0) {
        position = vec2(-1.0);
        uv = vec2(0.0);
    }else if(gl_VertexID == 1) {
        position = vec2(-1.0, 1.0);
        uv = vec2(0.0, 1.0);
    }else if(gl_VertexID == 2) {
        position = vec2(1.0, -1.0);
        uv = vec2(1.0, 0.0);
    }else if(gl_VertexID == 3) {
        position = vec2(1.0);
        uv = vec2(1.0);
    }

    gl_Position = vec4(position, 0.0, 1.0);
}
