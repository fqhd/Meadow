#pragma once
#include "Player.hpp"
#include "World.hpp"
#include "Camera.hpp"
#include "GameState.hpp"
#include "BlockOutline.hpp"
#include "Skybox.hpp"
#include <chrono>


class Game {
public:

	void init(Block* data, unsigned int worldSize);
	void update(float dt, GameState& state);
	void render();
	void destroy();

	World world;
	Player player;
	Camera camera;
	BlockOutline outline;
	Skybox skybox;
	bool toggleHud = true;

};