#pragma once
#include "Player.hpp"
#include "World.hpp"
#include "Renderer.hpp"
#include "Camera.hpp"
#include "GameState.hpp"

class Game {
public:

	void init();
	void update(float dt, GameState& state);
	void render();
	void destroy();

	Renderer renderer;
	World world;
	Player player;

private:
	
	Camera m_camera;

};