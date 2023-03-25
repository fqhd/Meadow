#include "Game.hpp"
#include "Window.hpp"
#include "GUIRenderer.hpp"

void Game::init() {
	world.init();
	camera.init(WINDOW_WIDTH, WINDOW_HEIGHT);
	player.init(&camera, &world);
	camera.mouseSensitivity = 0.2f;
	shadowmap.init();
	gbuffer.init();
	ssao.init();
}

void Game::update(float dt, GameState& state) {
	if (InputManager::isKeyPressed(GLFW_KEY_ESCAPE)) {
		state = GameState::Pause;
		InputManager::setMouseGrabbed(false);
	}

	player.update(dt);
	camera.setPosition(player.getEyePos());
	camera.update();
}

void Game::render() {
	world.updateMeshes();
	shadowmap.generateShadowMap(&world);
	gbuffer.populateGBuffer(&world, &camera);
	ssao.generateSSAOTexture(gbuffer.gPosition, gbuffer.gNormal, camera.getProjectionMatrix());

	world.render(camera, shadowmap.texture, shadowmap.lightSpaceMatrix, shadowmap.lightPos * 10000.0f);
	
	// Crosshair
	GUIRenderer::drawRect(glm::vec4(956, 536, 8, 8), ColorRGBA8(0, 0, 0));
	player.hotbar.render();
	const float size = 300;
	GUIRenderer::drawRect(glm::vec4(0.0, 0.0, size, size), gbuffer.gPosition);
	GUIRenderer::drawRect(glm::vec4(size, 0.0, size, size), gbuffer.gNormal);
	GUIRenderer::drawRect(glm::vec4(0.0, size, size, size), gbuffer.gAlbedo);
	GUIRenderer::drawRect(glm::vec4(size, size, size, size), ssao.ssaoTexture);
}

void Game::destroy() {
	world.destroy();
}