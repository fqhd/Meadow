#pragma once
#include "GUIButton.hpp"
#include "GUIInputBox.hpp"
#include "Game.hpp"
#include "GameState.hpp"

class MainMenu {
public:

	void init(Game* world);
	void update(float dt, GameState& state);
	void render();

private:

	GUIInputBox input;
	GUIButton createButton;
	GUIButton loadButton;
	std::string m_errorMsg;
	Game* game;

};