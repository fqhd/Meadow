#include "MainMenu.hpp"
#include "Window.hpp"

const int BUTTON_WIDTH = 300;

void MainMenu::init(Game* g) {
	game = g;
	createButton.init(glm::vec4(410, 200, 500, 200), ColorRGBA8(197, 255, 71, 255));
	loadButton.init(glm::vec4(1010, 200, 500, 200), ColorRGBA8(197, 255, 71, 255));
	input.init(glm::vec4(560, 500, 800, 200.0f));
}

void MainMenu::update(float dt, GameState& state) {
	createButton.update(dt);
	loadButton.update(dt);
	input.update();

	if (createButton.isPressed()) {
		if (input.text != "") {
			game->world.createNew(input.text);
			for (int i = 0; i < 9; i++) {
				game->player.hotbar.colors[i] = ColorRGBA8();
			}
			game->player.position = glm::vec3(CHUNK_WIDTH / 2, 1, CHUNK_WIDTH / 2);
			game->world.blockOffset = glm::ivec3(CHUNK_WIDTH * 1024);
			InputManager::setMouseGrabbed(true);
			
			state = GameState::Game;
			input.text = "";
			m_errorMsg = "";
		}
		else {
			m_errorMsg = "Could not create world with empty name";
		}
	}

	if (loadButton.isPressed()) {
		if (game->world.load(input.text) == -1) {
			m_errorMsg = "Could not find world with name: " + input.text;
		}
		else {
			input.text = "";
			m_errorMsg = "";
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
