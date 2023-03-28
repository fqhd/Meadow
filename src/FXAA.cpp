#include "FXAA.hpp"

static GLuint vao;

void FXAA::init() {
	shader.load("fxaa");
	glGenVertexArrays(1, &vao);
}

void FXAA::render(GLuint texture) {
	shader.bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	shader.unbind();
}