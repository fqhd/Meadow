#pragma once

#include "GUIRenderer.hpp"

class GUIInputBox {
public:

	void init(const glm::vec4& destRect);
	void update();
	void render();
	std::string getText();

private:
	glm::vec4 m_destRect;
	std::string m_text = "";

};