#include "PauseMenu.hpp"
#include "GUIRenderer.hpp"
#include "Window.hpp"
#include "InputManager.hpp"

void PauseMenu::init() {

}

void PauseMenu::update(GameState& state) {
	if (InputManager::isKeyPressed(GLFW_KEY_ESCAPE)) {
		state = GameState::Game;
		InputManager::setMouseGrabbed(true);
	}
}

void PauseMenu::render() {
	GUIRenderer::drawRect(glm::vec4(0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT), ColorRGBA8(0, 0, 0, 64));
	GUIRenderer::drawRect(glm::vec4(96, 96, 1728, 888), ColorRGBA8(255, 255, 255));
	GUIRenderer::drawRect(glm::vec4(100, 100, 1720, 880), ColorRGBA8(150, 150, 150));
}
