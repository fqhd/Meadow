#pragma once
#include "GUIButton.hpp"
#include "GUIInputBox.hpp"

class MainMenu {
public:

	void init();
	void update(float dt);
	void render();
	void destroy();

private:

	GUIInputBox input;
	GUIButton createButton;
	GUIButton loadButton;

};