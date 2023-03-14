#include "Game.hpp"
#include "Window.hpp"

void Game::init() {
	m_world.init();
	m_camera.init(WINDOW_WIDTH, WINDOW_HEIGHT);
	m_player.init(&m_camera, &m_world);
	m_camera.mouseSensitivity = 0.2f;

}

void Game::update(float dt) {
	m_player.update(dt);
	m_camera.setPosition(m_player.getEyePos());
	m_camera.update();
}

void Game::render() {
	m_world.render(m_camera);
}

void Game::destroy() {
	m_world.destroy();
}