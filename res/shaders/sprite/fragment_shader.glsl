#version 330 core

in vec2 uv;

out vec4 out_color;

uniform vec4 color[4];

uniform int type;
uniform sampler2D ourTexture;

vec4 biLerp(vec4 a, vec4 b, vec4 c, vec4 d, float s, float t)
{
  vec4 x = mix(a, b, t);
  vec4 y = mix(c, d, t);
  return mix(x, y, s);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main() {
	if(type == 0){ // TEXT
		float a = texture(ourTexture, uv).r;
		out_color = vec4(color[0].rgb, color[0].a * a);
	}else if(type == 1){ // RAINBOW
		out_color = vec4(hsv2rgb(vec3(uv.x, 1.0, 1.0)), 1.0);
	}else if(type == 2){ // COLORED QUAD
		out_color = biLerp(color[0], color[1], color[2], color[3], uv.x, uv.y);
	}else if(type == 3) { // TEXTURED QUAD
		out_color = vec4(texture(ourTexture, uv).xyz, 1.0);
	}
}
