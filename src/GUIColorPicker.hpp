#pragma once

#include "ColorRGBA8.hpp"

class GUIColorPicker {
public:
	
	void init();
	void render();

	ColorRGBA8 currentSliderColor;
	ColorRGBA8 selectedColor;

};