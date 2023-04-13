#include "BlockOutline.hpp"


void BlockOutline::init(){

	shader.load("block_outline");

	glGenVertexArrays(1, &m_vaoID);
	glBindVertexArray(m_vaoID);

	glGenBuffers(1, &m_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

	float vertices[] = {
		-0.001f, -0.001f, -0.001f,
		-0.001f, 1.001f, -0.001f,

		-0.001f, 1.001f, -0.001f,
		1.001f, 1.001f, -0.001f,

		1.001f, 1.001f, -0.001f,
		1.001f, -0.001f, -0.001f,

		1.001f, -0.001f, -0.001f,
		-0.001f, -0.001f, -0.001f,

		-0.001f, -0.001f, -0.001f,
		-0.001f, -0.001f, 1.001f,

		-0.001f, 1.001f, -0.001f,
		-0.001f, 1.001f, 1.001f,

		1.001f, 1.001f, -0.001f,
		1.001f, 1.001f, 1.001f,

		1.001f, -0.001f, -0.001f,
		1.001f, -0.001f, 1.001f,

		-0.001f, -0.001f, 1.001f,
		-0.001f, 1.001f, 1.001f,

		-0.001f, 1.001f, 1.001f,
		1.001f, 1.001f, 1.001f,

		1.001f, 1.001f, 1.001f,
		1.001f, -0.001f, 1.001f,

		1.001f, -0.001f, 1.001f,
		-0.001f, -0.001f, 1.001f,
	};

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, 0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void BlockOutline::render(const Camera& camera, const Player& player){
	if (!player.visibleBlocks.lookingAtBlock) return;

	shader.bind();
	shader.loadUniform("projection", camera.getProjectionMatrix());
	shader.loadUniform("view", camera.getViewMatrix());
	shader.loadUniform("blockPosition", player.visibleBlocks.breakableBlock);
	glBindVertexArray(m_vaoID);
	glDrawArrays(GL_LINES, 0, 24);
	glBindVertexArray(0);
	shader.unbind();
}

void BlockOutline::destroy(){
	glDeleteVertexArrays(1, &m_vaoID);
	glDeleteBuffers(1, &m_vboID);
}