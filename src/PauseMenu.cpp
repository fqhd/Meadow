#include "PauseMenu.hpp"
#include "Window.hpp"
#include "InputManager.hpp"

void PauseMenu::init(World* world) {
	m_world = world;
	saveButton.init(glm::vec4(540, 150, 400, 150), ColorRGBA8(197, 255, 71, 255));
	exitButton.init(glm::vec4(980, 150, 400, 150), ColorRGBA8(197, 255, 71, 255));
	colorPicker.init();
}

void PauseMenu::update(float deltaTime, GameState& state) {
	exitButton.update(deltaTime);
	saveButton.update(deltaTime);
	if (InputManager::isKeyPressed(GLFW_KEY_ESCAPE)) {
		state = GameState::Game;
		InputManager::setMouseGrabbed(true);
	}
	if (saveButton.isPressed()) {
		m_world->save();
	}
	if (exitButton.isPressed()) {
		state = GameState::MainMenu;
		InputManager::setMouseGrabbed(false);
	}
}

void PauseMenu::render() {
	GUIRenderer::drawRect(glm::vec4(0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT), ColorRGBA8(0, 0, 0, 64));
	GUIRenderer::drawRect(glm::vec4(96, 96, 1728, 888), ColorRGBA8(255, 255, 255));
	GUIRenderer::drawRect(glm::vec4(100, 100, 1720, 880), ColorRGBA8(52, 198, 235));
	saveButton.render();
	exitButton.render();
	colorPicker.render();
	GUIRenderer::drawText("Save", glm::vec2(600, 190), glm::vec2(3.0f), ColorRGBA8(255, 255, 255));
	GUIRenderer::drawText("Exit", glm::vec2(1080, 190), glm::vec2(3.0f), ColorRGBA8(255, 255, 255));
	GUIRenderer::drawText(m_world->getName(), glm::vec2(840, 900), glm::vec2(2.0f), ColorRGBA8(255, 255, 255));
}
