#include "MainMenu.hpp"
#include "Window.hpp"

const int BUTTON_WIDTH = 300;

void MainMenu::init() {
	createButton.init(glm::vec4(410, 200, 500, 200), ColorRGBA8(197, 255, 71, 255));
	loadButton.init(glm::vec4(1010, 200, 500, 200), ColorRGBA8(197, 255, 71, 255));
	input.init(glm::vec4(560, 500, 800, 200.0f));
}

void MainMenu::update(float dt) {
	createButton.update(dt);
	loadButton.update(dt);
	input.update();
}

void MainMenu::render() {
	GUIRenderer::drawRect(glm::vec4(0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT), ColorRGBA8(52, 198, 235));
	loadButton.render();
	createButton.render();
	input.render();
	GUIRenderer::drawText("Create", glm::vec2(460.0f, 270.0f), glm::vec2(3.0f), ColorRGBA8(255, 255, 255, 255));
	GUIRenderer::drawText("Load", glm::vec2(1120.0f, 270.0f), glm::vec2(3.0f), ColorRGBA8(255, 255, 255, 255));
}

void MainMenu::destroy() {

}