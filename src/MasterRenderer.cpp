#include "MasterRenderer.hpp"

static GLuint vao;

void MasterRenderer::init() {
	glGenVertexArrays(1, &vao);
	shader.load("master_render");
	shader.bind();
	shader.loadUniform("gPosition", 0);
	shader.loadUniform("gNormal", 1);
	shader.loadUniform("gAlbedo", 2);
	shader.loadUniform("ssao", 3);
	shader.loadUniform("depthmap", 4);
}

void MasterRenderer::render(GBuffer* gbuffer, ShadowMap* smap, GLuint ssao, Camera* camera) {
	shader.bind();

	shader.loadUniform("lightPos", smap->lightPos * 10000.0f);
	shader.loadUniform("lightSpaceMatrix", smap->lightSpaceMatrix);
	shader.loadUniform("view", camera->getViewMatrix());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gbuffer->gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gbuffer->gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gbuffer->gAlbedo);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, ssao);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, smap->texture);

	glBindVertexArray(vao);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
}