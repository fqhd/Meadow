#include "GUIInput.hpp"

void GUIInput::init(const glm::vec4& destRect,
				   	const ColorRGBA8& backgroundColor, 
	          		const ColorRGBA8& outlineColor, 
			  		const ColorRGBA8& activeOutlineColor) {
	m_outlineRect = glm::vec4(destRect.x-5, destRect.y-5, destRect.z+10, destRect.w+10);
	m_destRect = destRect;
	m_onFocus = false;
	m_input = "";

	m_backgroundColor = backgroundColor;
	m_outlineColor = outlineColor;
	m_activeOutlineColor = activeOutlineColor;
}

void GUIInput::update(){
	glm::ivec2 mousePos = glm::floor(InputManager::getScaledMousePosition());
	if (InputManager::isKeyPressed(GLFW_MOUSE_BUTTON_LEFT)) {
		if (Utils::isInside(mousePos, m_destRect)) {
			m_onFocus = m_onFocus ? false : true;
		}
		else {
			m_onFocus = false;
		}
	}
	if (m_onFocus) {
		char ascii_key = InputManager::getLastKeyPressed();
		if (ascii_key <= -2 && m_input.length() > 0) {
			m_input.pop_back();
		}
		else if (ascii_key > 0) {
			m_input.push_back(ascii_key);
		}
	}
	m_wasSubmitted = m_onFocus && InputManager::isKeyPressed(GLFW_KEY_ENTER);
}

bool GUIInput::wasSubmitted() {
	return m_wasSubmitted;
}

std::string GUIInput::getText() {
	return m_input;
}

void GUIInput::focus() {
	m_onFocus = true;
}

void GUIInput::render() {
	ColorRGBA8 outlineColor = m_onFocus ? m_activeOutlineColor : m_outlineColor;
	GUIRenderer::drawRect(m_outlineRect, outlineColor);

	GUIRenderer::drawRect(m_destRect, m_backgroundColor);
	GUIRenderer::drawText(m_input, glm::vec2(m_destRect.x + 3, m_destRect.y + 5), glm::vec2(0.75, 0.75), ColorRGBA8());
}