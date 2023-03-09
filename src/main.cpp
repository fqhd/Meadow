#include "Window.hpp"
#include "InputManager.hpp"
#include "GUIRenderer.hpp"

int main() {

	Window::create(1280, 720, "Meadow");
	InputManager::init(Window::getWindowPtr());
	GUIRenderer::init(1280, 720);

	while (InputManager::processInput()) {
		Window::clear();

		GUIRenderer::drawRect(glm::vec4(30.0f, 100.0f, 50.0f, 50.0f), ColorRGBA8(255, 0, 255));
		GUIRenderer::drawText("Hello, World!", glm::vec2(30.0f, 150.0f), glm::vec2(1.0f), ColorRGBA8(240, 44, 88));
		GUIRenderer::render();

		Window::update();
	}
	Window::close();

	return 0;
}