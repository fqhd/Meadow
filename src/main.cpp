#include "Window.hpp"
#include "InputManager.hpp"
#include "GUIRenderer.hpp"
#include "Game.hpp"

int main() {

	Window::create();
	InputManager::init(Window::getWindowPtr());
	InputManager::setMouseGrabbed(true);
	GUIRenderer::init(1280, 720);

	Game game;
	game.init();

	double before = glfwGetTime();

	while (InputManager::processInput()) {
		Window::clear();
		double dt = glfwGetTime() - before;
		before = glfwGetTime();

		GUIRenderer::drawRect(glm::vec4(30.0f, 100.0f, 50.0f, 50.0f), ColorRGBA8(255, 0, 255));
		GUIRenderer::drawText("Hello, World!", glm::vec2(30.0f, 150.0f), glm::vec2(1.0f), ColorRGBA8(240, 44, 88));
		GUIRenderer::render();

		game.update(dt);
		game.render();

		Window::update();
	}
	Window::close();

	return 0;
}