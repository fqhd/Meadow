#pragma once
#include "GUIRenderer.hpp"

class Hotbar {
public:

	void update();
	void render();

	ColorRGBA8 colors[9];
	int selectorIndex = 0;

};