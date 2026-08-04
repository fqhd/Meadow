#include "Hotbar.hpp"
#include "InputManager.hpp"


void Hotbar::update() {
	selectorIndex -= InputManager::getDeltaMouseWheel();
	if (selectorIndex < 0) {
		selectorIndex = 8;
	}
	selectorIndex %= 9;
}

void Hotbar::render() {
	GUIRenderer::drawRect(glm::vec4(596, 8, 728, 108), ColorRGBA8(87, 38, 0));
	for (int i = 0; i < 9; i++) {
		GUIRenderer::drawRect(glm::vec4(608 + i * 80, 24, 64, 64), colors[i]);
	}
	GUIRenderer::drawRect(glm::vec4(604 + selectorIndex * 80, 94, 72, 16), ColorRGBA8(0, 0, 0));

}