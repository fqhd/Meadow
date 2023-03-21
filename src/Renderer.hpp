#pragma once

#include "glad.h"
#include "World.hpp"
#include "Shader.hpp"
#include "Window.hpp"

class Renderer {
public:

	void init();
	void generateShadowMap(World* world);

	GLuint shadowmap;
	glm::mat4 lightSpaceMatrix;
	glm::vec3 lightPos;

public:
	
	GLuint framebuffer;
	Shader shader;

};