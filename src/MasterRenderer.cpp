#include "MasterRenderer.hpp"

static GLuint vao;

void MasterRenderer::init() {
	glGenVertexArrays(1, &vao);
	shader.load("chunk");
	shader.bind();
	shader.loadUniform("depthmap", 0);
	shader.loadUniform("aoTexture", 1);

	// Create texture data
	unsigned char* textureData = new unsigned char[2 * 2 * 4];

	textureData[0] = 20;
	textureData[1] = 40;
	textureData[2] = 60;
	textureData[3] = 80;

	textureData[4] = 100;
	textureData[5] = 120;
	textureData[6] = 140;
	textureData[7] = 160;



	textureData[8] = 180;
	textureData[9] = 200;
	textureData[10] = 220;
	textureData[11] = 240;

	textureData[12] = 10;
	textureData[13] = 0;
	textureData[14] = 250;
	textureData[15] = 150;

	// TODO: try with mipmapping
	glGenTextures(1, &textureArray);
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RED, 2, 2, 4, 0, GL_RED, GL_UNSIGNED_BYTE, textureData);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	delete[] textureData;
}

void MasterRenderer::render(ShadowMap* smap, Camera* camera, World* world) {
	shader.bind();

	shader.loadUniform("lightPos", smap->lightPos * 10000.0f);
	shader.loadUniform("lightSpaceMatrix", smap->lightSpaceMatrix);
	shader.loadUniform("view", camera->getViewMatrix());
	shader.loadUniform("projection", camera->getProjectionMatrix());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, smap->texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);

	world->chunk.render();

}