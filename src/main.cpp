#include "Window.hpp"
#include "InputManager.hpp"

int main() {

	Window::create(1280, 720, "Meadow");
	InputManager::init(Window::getWindowPtr());
	while (InputManager::processInput()) {
		Window::clear();

		Window::update();
	}
	Window::close();

	return 0;
}