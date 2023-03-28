#version 330 core

in vec2 uv;

uniform sampler2D ssaoInput;

out float FragColor;

void main() 
{
    vec2 texelSize = 1.0 / vec2(textureSize(ssaoInput, 0));
    float result = 0.0;
    for (int x = -3; x <= 3; ++x) 
    {
        for (int y = -3; y <= 3; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(ssaoInput, uv + offset).r;
        }
    }
    FragColor = result / 49.0;
}  