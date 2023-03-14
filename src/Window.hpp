#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GLAD/glad.h>
#include <glm/glm.hpp>
#include <iostream>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

namespace Window {
	void create();
	void clear();
	void update();
	void close();
	GLFWwindow* getWindowPtr();
}