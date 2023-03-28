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


	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glGenVertexArrays(1, &vao);
}

void MasterRenderer::generate(GBuffer* gbuffer, ShadowMap* smap, GLuint ssao, Camera* camera) {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_COLOR_BUFFER_BIT);

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

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}