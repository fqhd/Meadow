#pragma once


#include "Shader.hpp"
#include "Utils.hpp"
#include <glm/gtc/matrix_transform.hpp>

class GUIRenderer {
public:

	static void init(unsigned int windowWidth, unsigned int windowHeight);
	static void drawRect(const glm::vec4& destRect, const ColorRGBA8& color);
	static void drawRect(const glm::vec4& destRect, GLuint texture);
	static void drawRect(const glm::vec4& destRect, ColorRGBA8 v1, ColorRGBA8 v2, ColorRGBA8 v3, ColorRGBA8 v4);
	static void drawRainbow(const glm::vec4& destRect);
	static void drawText(const std::string& s, const glm::vec2& position, const glm::vec2& scale, const ColorRGBA8& color);

};
