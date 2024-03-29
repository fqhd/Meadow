#pragma once
#include "Player.hpp"
#include "World.hpp"
#include "ShadowMap.hpp"
#include "Camera.hpp"
#include "GameState.hpp"
#include "BlockOutline.hpp"
#include "Skybox.hpp"
#include <chrono>


class Game {
public:

	void init();
	void update(float dt, GameState& state);
	void render();
	void destroy();

	ShadowMap shadowmap;
	World world;
	Player player;
	Camera camera;
	BlockOutline outline;
	Skybox skybox;
	bool toggleHud = true;

};