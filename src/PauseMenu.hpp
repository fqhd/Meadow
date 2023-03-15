#pragma once
#include "GameState.hpp"
#include "GUIButton.hpp"
#include "World.hpp"

class PauseMenu {
public:

	void init(World* world);
	void update(float deltaTime, GameState& state);
	void render();

private:
	
	GUIButton exitButton;
	GUIButton saveButton;
	World* m_world;

};