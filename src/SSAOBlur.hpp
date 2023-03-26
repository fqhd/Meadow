#pragma once

#include "Shader.hpp"
#include "Window.hpp"

class SSAOBlur {
public:

	void init();
	void blurTexture(GLuint texture);


	GLuint blurredTexture;
	GLuint fbo;
	Shader shader;

};