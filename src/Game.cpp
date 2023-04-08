#include "Game.hpp"
#include "Window.hpp"
#include "GUIRenderer.hpp"

void Game::init() {
	world.init();
	camera.init(WINDOW_WIDTH, WINDOW_HEIGHT);
	player.init(&camera, &world);
	camera.mouseSensitivity = 0.2f;
}

void Game::update(float dt, GameState& state) {
	if (InputManager::isKeyPressed(GLFW_KEY_ESCAPE)) {
		state = GameState::Pause;
		InputManager::setMouseGrabbed(false);
	}

	player.update(dt);
	camera.setPosition(player.getEyePos());
	camera.update();

	t += dt;
	frames++;
	if (t >= 1.0) {
		std::cout << frames << std::endl;
		frames = 0;
		t = 0.0;
	}
}

void Game::render() {
	world.updateMeshes();

	world.render(camera, shadowmap.texture, shadowmap.lightSpaceMatrix, shadowmap.lightPos);

	GUIRenderer::drawRect(glm::vec4(956, 536, 8, 8), ColorRGBA8(0, 0, 0));
	player.hotbar.render();
}

void Game::destroy() {
	world.destroy();
}