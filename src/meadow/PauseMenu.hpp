#pragma once
#include "GameState.hpp"
#include "GUIButton.hpp"
#include "World.hpp"
#include "GUIColorPicker.hpp"
#include "Hotbar.hpp"

class PauseMenu {
public:

	void init(World* world);
	void update(float deltaTime, GameState& state);
	void render(Hotbar* hotbar);

private:
	
	GUIColorPicker colorPicker;
	GUIButton exitButton;
	GUIButton saveButton;
	World* m_world;
	int selectedSlot = 0;

};