#pragma once

#include "Shader.hpp"

class FXAA {
public:

	void init();
	void render(GLuint texture);

	Shader shader;

};