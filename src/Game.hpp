#pragma once
#include "Player.hpp"
#include "World.hpp"
#include "Camera.hpp"

class Game {
public:

	void init();
	void update(float dt);
	void render();
	void destroy();

private:
	
	Camera m_camera;
	World m_world;
	Player m_player;

};