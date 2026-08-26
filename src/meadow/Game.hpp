#pragma once
#include "Player.hpp"
#include "World.hpp"
#include "Camera.hpp"
#include "GameState.hpp"
#include "BlockOutline.hpp"
#include "Skybox.hpp"
#include <chrono>
#include <random>

class Game {
public:

	void init(Block* data, unsigned int worldSize);
	void update(float dt, GameState& state);
	void render();
	void destroy();
	void runDLA();
	void saveRenderState(const std::string& path);

	World world;
	Player player;
	Camera camera;
	BlockOutline outline;
	Skybox skybox;
	bool toggleHud = true;
	bool recording = false;
	bool dla = false;
	int frame = 0;
	std::mt19937 rng;

};
