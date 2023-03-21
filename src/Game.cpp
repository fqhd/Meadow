#include "Game.hpp"
#include "Window.hpp"
#include "GUIRenderer.hpp"

void Game::init() {
	world.init();
	m_camera.init(WINDOW_WIDTH, WINDOW_HEIGHT);
	m_player.init(&m_camera, &world);
	m_camera.mouseSensitivity = 0.2f;
	renderer.init();
}

void Game::update(float dt, GameState& state) {
	if (InputManager::isKeyPressed(GLFW_KEY_ESCAPE)) {
		state = GameState::Pause;
		InputManager::setMouseGrabbed(false);
	}

	m_player.update(dt);
	m_camera.setPosition(m_player.getEyePos());
	m_camera.update();
}

void Game::render() {
	world.updateMeshes();
	renderer.generateShadowMap(&world);
	world.render(m_camera, renderer.shadowmap, renderer.lightSpaceMatrix, -renderer.lightPos * 10000.0f);

	// Crosshair
	GUIRenderer::drawRect(glm::vec4(956, 536, 8, 8), ColorRGBA8(0, 0, 0));
}

void Game::destroy() {
	world.destroy();
}