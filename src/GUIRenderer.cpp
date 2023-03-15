#include "GUIRenderer.hpp"

SpriteBatch m_guiBatch;
SpriteBatch m_textBatch;
SpriteFont m_spriteFont;
Shader m_shader;
GLuint m_tmp_gui_texture;

void GUIRenderer::init(unsigned int windowWidth, unsigned int windowHeight){
	m_spriteFont.init("res/fonts/minecraft_font.ttf", 40, 512, 512);

	glGenTextures(1, &m_tmp_gui_texture);
	glBindTexture(GL_TEXTURE_2D, m_tmp_gui_texture);
	unsigned char data[] = {
		255, 255, 255, 255
	};
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
	m_guiBatch.init(m_tmp_gui_texture);
	m_textBatch.init(m_spriteFont.getTextureID());

	glm::mat4 ortho = glm::ortho(0.0f, (float)windowWidth, 0.0f, (float)windowHeight);

	m_shader.load("sprite");
	m_shader.bind();
	m_shader.loadUniform("matrix", ortho);
	m_shader.unbind();
}

void GUIRenderer::drawRect(const glm::vec4& destRect, const ColorRGBA8& color){
	m_guiBatch.draw(destRect, glm::vec4(0.0f), color);
}

void GUIRenderer::drawText(const std::string& s, const glm::vec2& position, const glm::vec2& scale, const ColorRGBA8& color){
	m_spriteFont.printFont(m_textBatch, s, position, scale, color);
}

void GUIRenderer::render(){
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	m_guiBatch.batch();
	m_textBatch.batch();
	
	m_shader.bind();
	m_shader.loadUniform("isFont", false);
	m_guiBatch.render();
	m_shader.loadUniform("isFont", true);
	m_textBatch.render();
	m_shader.unbind();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void GUIRenderer::destroy(){
	m_guiBatch.destroy();
	m_textBatch.destroy();
	m_shader.destroy();
	m_spriteFont.destroy();
}
