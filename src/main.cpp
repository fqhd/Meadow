#include "Window.hpp"
#include "InputManager.hpp"
#include "GUIRenderer.hpp"
#include "Camera.hpp"
#include "World.hpp"

int main() {

	Window::create(1280, 720, "Meadow");
	InputManager::init(Window::getWindowPtr());
	InputManager::setMouseGrabbed(true);
	GUIRenderer::init(1280, 720);
	Camera camera;
	camera.init(1280, 720);
	World world;
	world.init();
	camera.mouseSensitivity = 0.2f;

	for (int i = 0; i < WORLD_WIDTH * CHUNK_WIDTH; i++) {
		for (int j = 0; j < WORLD_WIDTH * CHUNK_WIDTH; j++) {
			world.setBlock(i, 0, j, Block(255, 0, 0));
		}
	}

	while (InputManager::processInput()) {
		Window::clear();

		GUIRenderer::drawRect(glm::vec4(30.0f, 100.0f, 50.0f, 50.0f), ColorRGBA8(255, 0, 255));
		GUIRenderer::drawText("Hello, World!", glm::vec2(30.0f, 150.0f), glm::vec2(1.0f), ColorRGBA8(240, 44, 88));
		GUIRenderer::render();

		camera.update();
		world.render(camera);

		Window::update();
	}
	Window::close();

	return 0;
}