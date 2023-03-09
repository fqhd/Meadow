#pragma once

#include "SpriteBatch.hpp"
#include "Shader.hpp"
#include "SpriteFont.hpp"
#include "Utils.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace GUIRenderer {

	void init(unsigned int windowWidth, unsigned int windowHeight);
	void drawRect(const glm::vec4& destRect, const ColorRGBA8& color);
	void drawText(const std::string& s, const glm::vec2& position, const glm::vec2& scale, const ColorRGBA8& color);
	void render();
	void destroy();

};
