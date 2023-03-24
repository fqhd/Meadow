#include "Game.hpp"
#include "Window.hpp"
#include "GUIRenderer.hpp"

void Game::init() {
	world.init();
	m_camera.init(WINDOW_WIDTH, WINDOW_HEIGHT);
	player.init(&m_camera, &world);
	m_camera.mouseSensitivity = 0.2f;
	shadowmap.init();
}

void Game::update(float dt, GameState& state) {
	if (InputManager::isKeyPressed(GLFW_KEY_ESCAPE)) {
		state = GameState::Pause;
		InputManager::setMouseGrabbed(false);
	}

	player.update(dt);
	m_camera.setPosition(player.getEyePos());
	m_camera.update();
}

void Game::render() {
	world.updateMeshes();
	shadowmap.generateShadowMap(&world);
	world.render(m_camera, shadowmap.texture, shadowmap.lightSpaceMatrix, shadowmap.lightPos * 10000.0f);
	
	// Crosshair
	GUIRenderer::drawRect(glm::vec4(956, 536, 8, 8), ColorRGBA8(0, 0, 0));
	player.hotbar.render();
}

void Game::destroy() {
	world.destroy();
}