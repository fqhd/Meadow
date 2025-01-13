#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "glad.h"
#include <glm/glm.hpp>
#include <iostream>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

class Window {
public:
	static void create();
	static void clear();
	static void update();
	static void close();
	static GLFWwindow* getWindowPtr();
};
