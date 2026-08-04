#include "Game.hpp"
#include "Window.hpp"
#include "GUIRenderer.hpp"

void Game::init(Block* data, unsigned int worldSize) {
	world.init(data, worldSize);
	player.init(&camera, &world);
	camera.init(WINDOW_WIDTH, WINDOW_HEIGHT, player.getEyePos());
	outline.init();
	skybox.init();
}

void Game::saveRenderState() {
    std::string fileName = Utils::generateRandomString(12);
    fileName += ".txt";
    std::string contents = "";
    contents += "CameraPosition: ";
    contents += std::to_string(camera.getPosition().x) + " " + std::to_string(camera.getPosition().y) + " " + std::to_string(camera.getPosition().z);
    contents += "\nCameraDirection: ";
    contents += std::to_string(camera.getForward().x) + " " + std::to_string(camera.getForward().y) + " " + std::to_string(camera.getForward().z);
    contents += "\nWorldData: " + world.getWorldDataBase64();
    Utils::writeStringToFile(contents, fileName);
    std::cout << "Render state saved: " << fileName << std::endl;
}

void Game::update(float dt, GameState& state) {
	if (InputManager::isKeyPressed(GLFW_KEY_ESCAPE)) {
		state = GameState::Pause;
		InputManager::setMouseGrabbed(false);
	}

	if (InputManager::isKeyPressed(GLFW_KEY_F)) {
        saveRenderState();
	}

	player.update(dt);
	camera.setPosition(player.getEyePos());
	camera.update();
}

void Game::render() {
	if (InputManager::isKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
		player.sprinting = !player.sprinting;
	}
	if (InputManager::isKeyPressed(GLFW_MOUSE_BUTTON_3)) {
		glm::ivec3 blockPos = player.visibleBlocks.breakableBlock;
		Block block = world.getBlock(blockPos.x, blockPos.y, blockPos.z);
		ColorRGBA8 color(block.r, block.g, block.b);
		player.hotbar.colors[player.hotbar.selectorIndex] = color;
	}
	if (InputManager::isKeyPressed(GLFW_KEY_H)) {
		toggleHud = !toggleHud;
	}
	world.updateMeshes();
	skybox.render(camera);
	world.render(camera);
	if (player.visibleBlocks.lookingAtBlock) {
		outline.render(camera, player);
	}

	if (toggleHud) {
		GUIRenderer::drawRect(glm::vec4(956, 536, 8, 8), ColorRGBA8(0, 0, 0));
		player.hotbar.render();
	}
}

void Game::destroy() {
	world.destroy();
}
