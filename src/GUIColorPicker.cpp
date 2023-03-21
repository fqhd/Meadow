#include "GUIColorPicker.hpp"
#include "GUIRenderer.hpp"



void GUIColorPicker::init() {

}

void GUIColorPicker::update() {

}

void GUIColorPicker::render() {
	GUIRenderer::drawRect(glm::vec4(1000.0f, 650.0f, 200.0f, 200.0f), ColorRGBA8(255, 0, 0));

}