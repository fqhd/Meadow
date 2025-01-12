#include "GUIRenderer.hpp"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

static Shader shader;
static GLuint m_font_texture;
static stbtt_bakedchar* m_charData = nullptr;
static int bitmapWidth = 512;
static int bitmapHeight = 512;
static GLuint vao;

void GUIRenderer::init(unsigned int windowWidth, unsigned int windowHeight){
	unsigned char* fontData = Utils::readFileToBuffer("res/fonts/minecraft_font.ttf");
	uint8_t* bitmapBuffer = (uint8_t*)malloc(bitmapWidth * bitmapHeight);

	m_charData = (stbtt_bakedchar*)malloc(sizeof(stbtt_bakedchar) * 95);
	stbtt_BakeFontBitmap(fontData, 0, 40, bitmapBuffer, bitmapWidth, bitmapHeight, 32, 95, m_charData);

	glGenTextures(1, &m_font_texture);
	glBindTexture(GL_TEXTURE_2D, m_font_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmapWidth, bitmapHeight, 0, GL_RED, GL_UNSIGNED_BYTE, bitmapBuffer);

	glBindTexture(GL_TEXTURE_2D, 0);

	free(bitmapBuffer);
	Utils::freeBuffer(fontData);

	glm::mat4 ortho = glm::ortho(0.0f, (float)windowWidth, 0.0f, (float)windowHeight);
	shader.load("sprite");
	shader.bind();
	shader.loadUniform("ortho", ortho);
	shader.unbind();
	glGenVertexArrays(1, &vao);
}

void GUIRenderer::drawRect(const glm::vec4& destRect, const ColorRGBA8& color) {
	shader.bind();
	shader.loadUniform("type", 2);
	shader.loadUniform("destRect", destRect);
	shader.loadUniform("uvRect", glm::vec4(0.0, 0.0, 1.0, 1.0));
	shader.loadUniform("color[0]", color);
	shader.loadUniform("color[1]", color);
	shader.loadUniform("color[2]", color);
	shader.loadUniform("color[3]", color);
	shader.loadUniform("ourTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_font_texture);
	glBindVertexArray(vao);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void GUIRenderer::drawRect(const glm::vec4& destRect, GLuint texture) {
	shader.bind();
	shader.loadUniform("type", 3);
	shader.loadUniform("destRect", destRect);
	shader.loadUniform("uvRect", glm::vec4(0.0, 0.0, 1.0, 1.0));
	shader.loadUniform("ourTexture", 0);
	glBindVertexArray(vao);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void GUIRenderer::drawRect(const glm::vec4& destRect, ColorRGBA8 v1, ColorRGBA8 v2, ColorRGBA8 v3, ColorRGBA8 v4) {
	shader.bind();
	shader.loadUniform("type", 2);
	shader.loadUniform("destRect", destRect);
	shader.loadUniform("uvRect", glm::vec4(0.0, 0.0, 1.0, 1.0));
	shader.loadUniform("color[0]", v1);
	shader.loadUniform("color[1]", v2);
	shader.loadUniform("color[2]", v4);
	shader.loadUniform("color[3]", v3);
	glBindVertexArray(vao);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void GUIRenderer::drawRainbow(const glm::vec4& destRect) {
	shader.bind();
	shader.loadUniform("type", 1);
	shader.loadUniform("destRect", destRect);
	shader.loadUniform("uvRect", glm::vec4(0.0, 0.0, 1.0, 1.0));
	glBindVertexArray(vao);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void flipQuad(glm::vec4& quad, float baseline) {
	float diff = baseline - (quad.y + quad.w);
	quad.y = baseline + diff;
}

void GUIRenderer::drawText(const std::string& s, const glm::vec2& position, const glm::vec2& scale, const ColorRGBA8& color){
	float xPos = position.x;
	float yPos = position.y;

	shader.bind();
	glBindVertexArray(vao);
	shader.loadUniform("ourTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_font_texture);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	shader.loadUniform("type", 0);
	shader.loadUniform("color[0]", color);
	for (unsigned int i = 0; i < s.size(); i++) {
		stbtt_aligned_quad q;

		stbtt_GetBakedQuad(m_charData, bitmapWidth, bitmapHeight, s[i] - 32, &xPos, &yPos, scale.x, &q, 1);

		float x = q.x0;
		float y = q.y0;
		float w = (q.x1 - q.x0);
		float h = (q.y1 - q.y0);
		glm::vec4 quad = glm::vec4(x, y, w, h);
		flipQuad(quad, yPos);
		quad.w *= scale.x;
		quad.z *= scale.y;

		shader.loadUniform("destRect", quad);
		glm::vec4 uvRect(q.s0, q.t0 + (q.t1 - q.t0), q.s1 - q.s0, -(q.t1 - q.t0));
		shader.loadUniform("uvRect", uvRect);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

