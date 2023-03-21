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

void PauseMenu::render(Hotbar* hotbar) {
	GUIRenderer::drawRect(glm::vec4(0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT), ColorRGBA8(0, 0, 0, 64));
	GUIRenderer::drawRect(glm::vec4(96, 96, 1728, 888), ColorRGBA8(255, 255, 255));
	GUIRenderer::drawRect(glm::vec4(100, 100, 1720, 880), ColorRGBA8(52, 198, 235));
	saveButton.render();
	exitButton.render();
	colorPicker.render();
	// colorPicker.work();
	GUIRenderer::drawRect(glm::vec4(604 + selectedSlot * 80, 346, 72, 72), ColorRGBA8(87, 38, 0));
	for (int i = 0; i < 9; i++) {
		GUIRenderer::drawRect(glm::vec4(608 + i*80, 350, 64, 64), hotbar->colors[i]);
		if (InputManager::isKeyDown(GLFW_MOUSE_BUTTON_1)) {
			if (Utils::isInside(InputManager::getMousePosition(), glm::vec4(608 + i * 80, 350, 64, 64))) {
				selectedSlot = i;
			}
		}
	}

	if (colorPicker.change) hotbar->colors[selectedSlot] = colorPicker.selectedColor;

	GUIRenderer::drawText("Save", glm::vec2(600, 190), glm::vec2(3.0f), ColorRGBA8(255, 255, 255));
	GUIRenderer::drawText("Exit", glm::vec2(1080, 190), glm::vec2(3.0f), ColorRGBA8(255, 255, 255));
	GUIRenderer::drawText(m_world->getName(), glm::vec2(840, 900), glm::vec2(2.0f), ColorRGBA8(255, 255, 255));
}
