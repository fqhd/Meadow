#pragma once

#include "glad.h"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Player.hpp"

class BlockOutline {
public:

	void init();
	void render(const Camera& camera, const Player& player);
	void destroy();

private:

	GLuint m_vaoID = 0;
	GLuint m_vboID = 0;
	Shader shader;

};