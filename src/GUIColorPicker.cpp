#include "GUIColorPicker.hpp"
#include "GUIRenderer.hpp"
#include "InputManager.hpp"
#include "Window.hpp"

glm::vec3 hsv2rgb(glm::vec3 c) {
	glm::vec4 K = glm::vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	glm::vec3 p = abs(glm::fract(glm::vec3(c.x, c.x, c.x) + glm::vec3(K.x, K.y, K.z)) * 6.0f - glm::vec3(K.w, K.w, K.w));
	return c.z * glm::mix(glm::vec3(K.x, K.x, K.x), glm::clamp(p - glm::vec3(K.x, K.x, K.x), 0.0f, 1.0f), c.y);
}

glm::vec4 biLerp(glm::vec4 a, glm::vec4 b, glm::vec4 c, glm::vec4 d, float s, float t)
{
	glm::vec4 x = glm::mix(a, b, t);
	glm::vec4 y = glm::mix(c, d, t);
	return glm::mix(x, y, s);
}

void GUIColorPicker::init() {
	currentSliderColor = ColorRGBA8(255, 0, 0);
	selectedColor = ColorRGBA8(255, 0, 0);
}

void GUIColorPicker::render() {
	GUIRenderer::drawRect(glm::vec4(1000.0f, 650.0f, 200.0f, 200.0f), selectedColor);
	GUIRenderer::drawRect(glm::vec4(1200.0f, 650.0f, 500.0f, 200.0f), ColorRGBA8(0, 0, 0), ColorRGBA8(), currentSliderColor, ColorRGBA8(0, 0 ,0));
	GUIRenderer::drawRainbow(glm::vec4(1000.0f, 600.0f, 700.0f, 50.0f));
	change = false;
	if (InputManager::isKeyDown(GLFW_MOUSE_BUTTON_1)) { // Select color based on hue
		const glm::ivec2 mousePos = InputManager::getScaledMousePosition();
		glm::vec4 scaledHueDestRect = Utils::toScreenCoords(glm::vec4(1000.0f, 600.0f, 700.0f, 50.0f), WINDOW_WIDTH, WINDOW_HEIGHT);
		if (Utils::isInside(mousePos, scaledHueDestRect)) {
			float y = mousePos.x - scaledHueDestRect.x;
			y /= scaledHueDestRect.z;
			glm::vec3 color = hsv2rgb(glm::vec3(y, 1.0f, 1.0f));
			currentSliderColor.r = color.r * 255;
			currentSliderColor.g = color.g * 255;
			currentSliderColor.b = color.b * 255;
			currentSliderColor.a = 255;
		}
		glm::vec4 scaledSVDestRect = Utils::toScreenCoords(glm::vec4(1200.0f, 650.0f, 500.0f, 200.0f), WINDOW_WIDTH, WINDOW_HEIGHT);
		if (Utils::isInside(mousePos, scaledSVDestRect)) {
			float x = mousePos.x - scaledSVDestRect.x;
			float y = mousePos.y - scaledSVDestRect.y;

			x /= scaledSVDestRect.z;
			y /= scaledSVDestRect.w;

			glm::vec4 currCol(0.0f);
			currCol.r = currentSliderColor.r / 255.0f;
			currCol.g = currentSliderColor.g / 255.0f;
			currCol.b = currentSliderColor.b / 255.0f;
			currCol.a = 1.0f;

			glm::vec4 color = biLerp(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), currCol, x, y);

			selectedColor.r = color.r * 255;
			selectedColor.g = color.g * 255;
			selectedColor.b = color.b * 255;
			selectedColor.a = 255;
			change = true;
		}
	}
}