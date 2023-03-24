#pragma once

#include "glad.h"
#include "World.hpp"
#include "Window.hpp"
#include "Shader.hpp"

class GBuffer {
public:

	void init();
	void populateGBuffer(World* world, Camera* camera);

	GLuint gPosition;
	GLuint gNormal;
	GLuint gAlbedo;

private:

	GLuint fbo;
	Shader shader;


};