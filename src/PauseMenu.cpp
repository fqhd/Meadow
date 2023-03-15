#include "PauseMenu.hpp"
#include "Window.hpp"
#include "InputManager.hpp"

void PauseMenu::init() {
	saveButton.init(glm::vec4(760, 150, 400, 150), ColorRGBA8(197, 255, 71, 255));
}

void PauseMenu::update(float deltaTime, GameState& state) {
	saveButton.update(deltaTime);
	if (InputManager::isKeyPressed(GLFW_KEY_ESCAPE)) {
		state = GameState::Game;
		InputManager::setMouseGrabbed(true);
	}
}

void PauseMenu::render() {
	GUIRenderer::drawRect(glm::vec4(0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT), ColorRGBA8(0, 0, 0, 64));
	GUIRenderer::drawRect(glm::vec4(96, 96, 1728, 888), ColorRGBA8(255, 255, 255));
	GUIRenderer::drawRect(glm::vec4(100, 100, 1720, 880), ColorRGBA8(150, 150, 150));
	saveButton.render();
	GUIRenderer::drawText("Save", glm::vec2(820, 190), glm::vec2(3.0f), ColorRGBA8(255, 255, 255));
}
