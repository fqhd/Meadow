#pragma once
#include "GameState.hpp"
#include "GUIButton.hpp"

class PauseMenu {
public:

	void init();
	void update(float deltaTime, GameState& state);
	void render();

private:
	
	GUIButton saveButton;

};