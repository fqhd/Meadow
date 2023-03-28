#version 330 core

in vec2 uv;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D ssao;
uniform sampler2D depthmap;

uniform vec3 lightPos;
uniform mat4 lightSpaceMatrix;
uniform mat4 view;

out vec4 FragColor;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
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
	vec3 normal = texture(gNormal, uv).xyz;
	vec3 position = texture(gPosition, uv).xyz;
	vec3 albedo = texture(gAlbedo, uv).xyz;
	
	normal = (inverse(view) * vec4(normal, 0.0)).xyz;
	position = (inverse(view) * vec4(position, 1.0)).xyz;
	
	vec4 lightSpacePos = lightSpaceMatrix * vec4(position, 1.0);
	
	vec3 lightDir = normalize(lightPos - position);
	
	float shadow = ShadowCalculation(lightSpacePos, normal, lightDir);
	
	float ssao = texture(ssao, uv).r;
	
	float brightness = max(dot(normal, lightDir), 0.2);
	brightness *= 1.5;
    brightness *= shadow;
	vec3 diffuse = albedo * clamp(brightness, 0.5, 1.0);
	
	FragColor = vec4(diffuse * ssao, 1.0);
}