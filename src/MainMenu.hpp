#pragma once
#include "GUIButton.hpp"

class MainMenu {
public:

	void init();
	void update(float dt);
	void render();
	void destroy();

private:

	GUIButton createButton;
	GUIButton loadButton;

};