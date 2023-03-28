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
	ssaoblur.init();
	renderer.init();
	fxaa.init();
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
	
	ssaoblur.blurTexture(ssao.ssaoTexture);

	renderer.generate(&gbuffer, &shadowmap, ssaoblur.blurredTexture, &camera);

	fxaa.render(renderer.texture);

	// Crosshair
	GUIRenderer::drawRect(glm::vec4(956, 536, 8, 8), ColorRGBA8(0, 0, 0));
	player.hotbar.render();

	// Debug
	const float size = 300;
	GUIRenderer::drawRect(glm::vec4(0.0, 0.0, size, size), gbuffer.gPosition);
	GUIRenderer::drawRect(glm::vec4(size, 0.0, size, size), gbuffer.gNormal);
	GUIRenderer::drawRect(glm::vec4(0.0, size, size, size), gbuffer.gAlbedo);
	GUIRenderer::drawRect(glm::vec4(size, size, size, size), ssao.ssaoTexture);
	GUIRenderer::drawRect(glm::vec4(0, size*2, size, size), ssaoblur.blurredTexture);
}

void Game::destroy() {
	world.destroy();
}