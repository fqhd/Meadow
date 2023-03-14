#include "MainMenu.hpp"
#include "Window.hpp"

const int BUTTON_WIDTH = 300;

void MainMenu::init(World* world) {
	m_world = world;
	createButton.init(glm::vec4(410, 200, 500, 200), ColorRGBA8(197, 255, 71, 255));
	loadButton.init(glm::vec4(1010, 200, 500, 200), ColorRGBA8(197, 255, 71, 255));
	input.init(glm::vec4(560, 500, 800, 200.0f));
}

void MainMenu::update(float dt, GameState& state) {
	createButton.update(dt);
	loadButton.update(dt);
	input.update();

	if (createButton.isPressed()) {
		m_world->createNew(input.getText());
		InputManager::setMouseGrabbed(true);
		state = GameState::Game;
	}

	if (loadButton.isPressed()) {
		if (m_world->load(input.getText()) == -1) {
			m_errorMsg = "Could not find world with name: " + input.getText();
		}
		else {
			InputManager::setMouseGrabbed(true);
			state = GameState::Game;
		}
	}
}

void MainMenu::render() {
	GUIRenderer::drawRect(glm::vec4(0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT), ColorRGBA8(52, 198, 235));
	loadButton.render();
	createButton.render();
	input.render();
	GUIRenderer::drawText("Create", glm::vec2(460.0f, 270.0f), glm::vec2(3.0f), ColorRGBA8(255, 255, 255, 255));
	GUIRenderer::drawText("Load", glm::vec2(1120.0f, 270.0f), glm::vec2(3.0f), ColorRGBA8(255, 255, 255, 255));
	GUIRenderer::drawText(m_errorMsg, glm::vec2(600, 80.0f), glm::vec2(1.0f), ColorRGBA8(255, 0, 0, 255));
}
