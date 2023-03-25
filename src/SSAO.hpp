#pragma once

#include "Shader.hpp"
#include "glad.h"
#include "Window.hpp"

class SSAO {
public:

	void init();
	void generateSSAOTexture(GLuint gPos, GLuint gNorm, const glm::mat4& proj);

	GLuint ssaoFBO;
	GLuint ssaoTexture;
	GLuint noiseTexture;
	Shader shader;

};