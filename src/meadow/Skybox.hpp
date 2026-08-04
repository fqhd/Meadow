#pragma once

#include "Shader.hpp"
#include "Camera.hpp"

class Skybox {
public:

	void init();
	void render(const Camera& camera);
	void destroy();

private:

	GLuint m_vaoID = 0;
	GLuint m_vboID = 0;
	Shader shader;

};