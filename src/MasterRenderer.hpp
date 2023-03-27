#pragma once

#include "World.hpp"
#include "Shader.hpp"
#include "GBuffer.hpp"
#include "ShadowMap.hpp"
#include "Camera.hpp"

class MasterRenderer {
public:

	void init();
	void render(GBuffer* gbuffer, ShadowMap* smap, GLuint ssao, Camera* camera);

	Shader shader;

};