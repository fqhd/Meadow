#pragma once
#include "Player.hpp"
#include "World.hpp"
#include "Camera.hpp"
#include "GameState.hpp"

class Game {
public:

	void init();
	void update(float dt, GameState& state);
	void render();
	void destroy();

	World world;

private:
	
	Camera m_camera;
	Player m_player;

};