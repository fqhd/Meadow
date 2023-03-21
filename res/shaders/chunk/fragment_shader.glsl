#version 330 core

in vec3 fragColor;
in vec4 fragPosLightSpace;
in vec3 normal;
in vec3 lightDir;

out vec4 out_color;

uniform sampler2D depthmap;

float ShadowCalculation()
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float currentDepth = projCoords.z;

    float bias = max(0.004 * (1.0 - dot(normal, lightDir)), 0.00001);  

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(depthmap, 0);
    for(int x = -2; x <= 2; ++x)
    {
        for(int y = -2; y <= 2; ++y)
        {
            float pcfDepth = texture(depthmap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 0.0 : 1.0;        
        }    
    }
    
    shadow /= 25.0;

    return shadow;
}  

void main() {
	float shadow = ShadowCalculation();

    float brightness = max(dot(normal, lightDir), 0.2);
    brightness *= 1.5;
    brightness *= shadow;

    vec3 diffuse = fragColor * clamp(brightness, 0.5, 1.0);

	out_color = vec4(diffuse, 1.0);
}
