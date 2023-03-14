#include "GUIInputBox.hpp"
#include "InputManager.hpp"

void GUIInputBox::init(const glm::vec4& destRect) {
	m_destRect = destRect;
}

void GUIInputBox::update() {
	char lastKeyPressed = InputManager::getLastKeyPressed();
	if (lastKeyPressed != -1) {
		if (lastKeyPressed == -2) {
			if (m_text.size()) {
				m_text.pop_back();
			}
		}
		else{
			m_text += lastKeyPressed;
		}
	}
}

std::string GUIInputBox::getText() {
	return m_text;
}

void GUIInputBox::render() {
	GUIRenderer::drawRect(glm::vec4(m_destRect.x - 4.0f, m_destRect.y - 4.0f, m_destRect.z + 8.0f, m_destRect.w + 8.0f) + glm::vec4(8, -8, 0, 0), ColorRGBA8(0, 0, 0, 128));
	GUIRenderer::drawRect(glm::vec4(m_destRect.x - 4.0f, m_destRect.y - 4.0f, m_destRect.z + 8.0f, m_destRect.w + 8.0f), ColorRGBA8(255, 255, 255));
	GUIRenderer::drawRect(m_destRect, ColorRGBA8(120, 59, 13));
	GUIRenderer::drawText(m_text, glm::vec2(m_destRect.x + 16.0f, m_destRect.y + 16.0f), glm::vec2(2.0f), ColorRGBA8(255, 255, 255));
}