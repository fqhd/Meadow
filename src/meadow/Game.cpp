#include "Game.hpp"
#include "Window.hpp"
#include "GUIRenderer.hpp"
#include "InputManager.hpp"

void Game::init(Block* data, unsigned int worldSize) {
	world.init(data, worldSize);
	player.init(&camera, &world);
	camera.init(WINDOW_WIDTH, WINDOW_HEIGHT, player.getEyePos());
	outline.init();
	skybox.init();
	world.setBlock(worldSize * 8, worldSize * 8, worldSize * 8, Block(255, 255, 255, true));
}

void Game::saveRenderState(const std::string& path) {

    std::string contents = "";
    contents += "CameraPosition: ";
    contents += std::to_string(camera.getPosition().x) + " " + std::to_string(camera.getPosition().y) + " " + std::to_string(camera.getPosition().z);
    contents += "\nCameraDirection: ";
    contents += std::to_string(camera.getForward().x) + " " + std::to_string(camera.getForward().y) + " " + std::to_string(camera.getForward().z);
    contents += "\nWorldSize: " + std::to_string(world.getWorldSize());
    contents += "\nWorldData: " + world.getWorldDataBase64();
    Utils::writeStringToFile(contents, path);
    std::cout << "Render state saved: " << path << std::endl;
}

void Game::runDLA() {
    std::uniform_int_distribution<int> dist(0, (world.getWorldSize() * 16) - 1);
    std::uniform_int_distribution<int> direction(0, 5);

    int x = dist(rng);
    int y = dist(rng);
    int z = dist(rng);


    while (true) {
        x %= world.getWorldSize() * 16;
        y %= world.getWorldSize() * 16;
        z %= world.getWorldSize() * 16;

        if (world.getBlock(x + 1, y, z).visible ||
            world.getBlock(x - 1, y, z).visible ||
            world.getBlock(x, y + 1, z).visible ||
            world.getBlock(x, y - 1, z).visible ||
            world.getBlock(x, y, z + 1).visible ||
            world.getBlock(x, y, z - 1).visible) {

            float cx = world.getWorldSize() * 8.0f;
            float cy = world.getWorldSize() * 8.0f;
            float cz = world.getWorldSize() * 8.0f;

            float dx = x - cx;
            float dy = y - cy;
            float dz = z - cz;

            float distance = std::sqrt(dx*dx + dy*dy + dz*dz);

            float t = distance * 0.03f;

            int r = (std::sin(t) * 0.5f + 0.5f) * 255;
            int g = (std::sin(t + 2.0f) * 0.5f + 0.5f) * 255;
            int b = (std::sin(t + 4.0f) * 0.5f + 0.5f) * 255;

            world.setBlock(x, y, z, Block(r, g, b, true));
            return;
        }

        switch (direction(rng)) {
            case 0: x++; break;
            case 1: x--; break;
            case 2: y++; break;
            case 3: y--; break;
            case 4: z++; break;
            case 5: z--; break;
        }
    }
}

void Game::update(float dt, GameState& state) {
	if (InputManager::isKeyPressed(GLFW_KEY_ESCAPE)) {
		state = GameState::Pause;
		InputManager::setMouseGrabbed(false);
	}

	if (InputManager::isKeyPressed(GLFW_KEY_F)) {
    	std::string fileName = Utils::generateRandomString(12);
        fileName += ".txt";
        saveRenderState(fileName);
	}

	if (InputManager::isKeyPressed(GLFW_KEY_R)) {
	    recording = !recording;
	}

	if (InputManager::isKeyPressed(GLFW_KEY_L)) {
	    dla = !dla;
	}

	if (dla) {
        runDLA();
	    runDLA();
	}

	if (recording) {
        std::string path = "recordings/frame_";
       	path += std::string(4 - std::to_string(frame).length(), '0') + std::to_string(frame);
       	path += ".txt";
		saveRenderState(path);
		frame++;
	}

	player.update(dt);
	camera.setPosition(player.getEyePos());
	camera.update(dt);
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
