#include "Window.hpp"
#include "InputManager.hpp"
#include "GUIRenderer.hpp"
#include "Game.hpp"
#include "GameState.hpp"
#include "PauseMenu.hpp"
#include <algorithm>


static bool isUnsignedInteger(const std::string& str) {
	return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

int main() {

	Block* data = nullptr;
	unsigned int worldSize;

	// Try to open the world save file, if it exists, load the world from it, if not, prompt the user for world size and create a new one
	std::ifstream file("world.dat", std::ios::in | std::ios::binary);
	if (!file.good()) {
		std::cout << "Could not find world save file, creating a new one..." << std::endl;
		std::cout << "Enter the world size(1-32): ";
		std::string userInput;
		std::getline(std::cin, userInput);
		if (!isUnsignedInteger(userInput)) {
			throw std::runtime_error("The world size must be an unsigned integer(a positive whole number)");
		}
		worldSize = std::stoi(userInput);
		if (worldSize > 32 || worldSize <= 0) {
			throw std::runtime_error("Invalid range. The world size must be between 1 and 32");
		}

		data = new Block[CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH * worldSize * worldSize * 4];
		memset(data, 0, sizeof(Block) * CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH * worldSize * worldSize * 4);
		for (int i = 0; i < CHUNK_WIDTH * worldSize; i++) {
			for (int j = 0; j < CHUNK_WIDTH * worldSize; j++) {
				data[i * CHUNK_WIDTH * worldSize + j] = Block(255, 255, 255, true);
			}
		}
	}
	else {
		file.read(reinterpret_cast<char*>(&worldSize), sizeof(worldSize));
		if (!file) {
			throw std::runtime_error("Failed to load world, file format invalid");
		}
		if (worldSize <= 0 || worldSize > 32) {
			throw std::runtime_error("Failed to load world, world size out of range");
		}

		data = new Block[CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH * worldSize * worldSize * 4];

		file.read(reinterpret_cast<char*>(data), CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH * worldSize * worldSize * 4);
		if (!file) {
			throw std::runtime_error("Failed to load world, insufficient world data");
		}

		file.close();
	}

	Window::create();
	InputManager::init(Window::getWindowPtr());
	GUIRenderer::init(WINDOW_WIDTH, WINDOW_HEIGHT);

	Game game;
	game.init(data, worldSize);
	PauseMenu pausemenu;
	pausemenu.init(&game.world);
	GameState gamestate = GameState::Pause;

	double before = glfwGetTime();
	while (gamestate != GameState::Exit) {
		Window::clear();
		double nowTime = glfwGetTime();
		double dt = nowTime - before;
		before = nowTime;
		if (!InputManager::processInput()) gamestate = GameState::Exit;

		if (gamestate == GameState::Game) {
			game.update(dt, gamestate);
			game.render();
		}
		else if (gamestate == GameState::Pause) {
			pausemenu.update(dt, gamestate);
			game.render();
			pausemenu.render(&game.player.hotbar);
		}

		Window::update();
	}

	game.destroy();
	Window::close();

	return 0;
}