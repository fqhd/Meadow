#pragma once
#include "GUIButton.hpp"
#include "GUIInputBox.hpp"
#include "World.hpp"
#include "GameState.hpp"

class MainMenu {
public:

	void init(World* world);
	void update(float dt, GameState& state);
	void render();
	void destroy();

private:

	GUIInputBox input;
	GUIButton createButton;
	GUIButton loadButton;
	std::string m_errorMsg;
	World* m_world;

};