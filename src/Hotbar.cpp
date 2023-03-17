#include "Hotbar.hpp"
#include "InputManager.hpp"


void Hotbar::update() {

}

void Hotbar::render() {
	GUIRenderer::drawRect(glm::vec4(772, 0, 376, 44), ColorRGBA8(255, 255, 255));
}