#pragma once
#include "GameState.hpp"
#include "GUIButton.hpp"
#include "World.hpp"
#include "GUIColorPicker.hpp"

class PauseMenu {
public:

	void init(World* world);
	void update(float deltaTime, GameState& state);
	void render();

private:
	
	GUIColorPicker colorPicker;
	GUIButton exitButton;
	GUIButton saveButton;
	World* m_world;

};