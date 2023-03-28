#pragma once

#include "World.hpp"
#include "Shader.hpp"
#include "GBuffer.hpp"
#include "ShadowMap.hpp"
#include "Camera.hpp"
#include "Window.hpp"

class MasterRenderer {
public:

	void init();
	void generate(GBuffer* gbuffer, ShadowMap* smap, GLuint ssao, Camera* camera);

	Shader shader;
	GLuint texture;
	GLuint fbo;

};