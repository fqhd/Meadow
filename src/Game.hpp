#pragma once
#include "Player.hpp"
#include "World.hpp"
#include "ShadowMap.hpp"
#include "Camera.hpp"
#include "GameState.hpp"
#include "GBuffer.hpp"
#include "SSAO.hpp"

class Game {
public:

	void init();
	void update(float dt, GameState& state);
	void render();
	void destroy();

	ShadowMap shadowmap;
	World world;
	Player player;
	GBuffer gbuffer;
	SSAO ssao;
	Camera camera;
	

};