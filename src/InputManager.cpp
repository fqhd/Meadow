#include "InputManager.hpp"
#include <iostream>

glm::vec2 windowSize;
glm::vec2 mousePosition;
glm::vec2 previousMousePosition;
std::unordered_map<int, bool> keymap;
bool isFocused = true;
char lastKeyPressed = -1;
float mouseWheelDelta = 0;

std::unordered_map<int, bool> InputManager::m_previousKeymap;
GLFWwindow* InputManager::m_window = nullptr;

void keyPressed(int _keyID) {
	keymap[_keyID] = true;
}

void keyReleased(int _keyID) {
	keymap[_keyID] = false;
}

void focusChangedCallback(GLFWwindow* window, int focused) {
	isFocused = focused;
}

void windowResizedCallback(GLFWwindow* window, int width, int height) {
	windowSize.x = width;
	windowSize.y = height;
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	mousePosition.x = x;
	mousePosition.y = windowSize.y - y;
	previousMousePosition = mousePosition;
}

void keyPressedCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods) {
	if (_action == GLFW_PRESS) {
		if (_key == 259) { // 259 is the keycode for the backspace. If we pressed on backspace we want to set lastKeyPressed to -2 to let anyone calling char getLastKeyPressed() know that the last key pressed was a backspace and not a normal char
			lastKeyPressed = -2;
		}
		keyPressed(_key);
	}
	else if (_action == GLFW_RELEASE) {
		keyReleased(_key);
	}
}

void keyTypedCallback(GLFWwindow* window, unsigned int codepoint) {
	if (codepoint >= 32 && codepoint <= 127) {
		lastKeyPressed = codepoint;
	}
}

void buttonPressedCallback(GLFWwindow* _window, int _button, int _action, int _mods) {
	if (_action == GLFW_PRESS) {
		keyPressed(_button);
	}
	else if (_action == GLFW_RELEASE) {
		keyReleased(_button);
	}
}

void mouseMovedCallback(GLFWwindow* window, double _xpos, double _ypos) {
	mousePosition = glm::vec2((float)_xpos, windowSize.y - (float)_ypos);
}

void wheelScrolledCallback(GLFWwindow* window, double xoffset, double yoffset) {
	mouseWheelDelta = yoffset;
}

void InputManager::init(GLFWwindow* _window) {
	m_window = _window;
	glfwSetKeyCallback(_window, keyPressedCallback);
	glfwSetMouseButtonCallback(_window, buttonPressedCallback);
	glfwSetCursorPosCallback(_window, mouseMovedCallback);
	glfwSetWindowFocusCallback(_window, focusChangedCallback);
	glfwSetCharCallback(_window, keyTypedCallback);
	glfwSetScrollCallback(_window, wheelScrolledCallback);
	glfwSetWindowSizeCallback(_window, windowResizedCallback);

	// We must also initalize the window size on init because the windowResized() callback only gets called when the window is resized so the width and height of the window are uninitialized in the start of the application.
	int w, h;
	glfwGetWindowSize(_window, &w, &h);
	windowSize = glm::vec2(w, h);

	// We must inialize the mouse position on init because the mouseMoved() callback function only sets the mouse position when the mouse position is moved so they are not initialized at the start of the application.
	double x, y;
	glfwGetCursorPos(_window, &x, &y);
	mousePosition = glm::vec2(x, h - y);
	previousMousePosition = mousePosition;
}

bool InputManager::processInput() {
	previousMousePosition = mousePosition;
	m_previousKeymap = keymap;
	lastKeyPressed = -1; // char getLastKeyPressed() returns -1 if no key was pressed
	mouseWheelDelta = 0.0f;

	glfwPollEvents();
	return !glfwWindowShouldClose(m_window);
}

float InputManager::getDeltaMouseWheel() {
	return mouseWheelDelta;
}

bool InputManager::hasFocus() {
	return isFocused;
}

char InputManager::getLastKeyPressed() {
	return lastKeyPressed;
}

void InputManager::setMouseGrabbed(bool _grabbed) {
	if (_grabbed) {
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else {
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void InputManager::setVerticalSync(bool _sync) {
	glfwSwapInterval(_sync);
}

glm::vec2 InputManager::getPreviousMousePosition() {
	return previousMousePosition;
}

glm::vec2 InputManager::getMousePosition() {
	return mousePosition;
}

glm::vec2 InputManager::getScaledMousePosition() {
	return mousePosition / windowSize * glm::vec2(1280.0f, 720.0f);
}

glm::vec2 InputManager::getWindowSize() {
	return windowSize;
}

bool InputManager::isKeyDown(int _keyID) {
	auto it = keymap.find(_keyID);
	if (it != keymap.end()) {
		return it->second;
	}
	return false;
}

bool InputManager::wasKeyDown(int _keyID) {
	auto it = m_previousKeymap.find(_keyID);
	if (it != m_previousKeymap.end()) {
		return it->second;
	}
	return false;
}

bool InputManager::isKeyPressed(int _keyID) {
	return (isKeyDown(_keyID) && !wasKeyDown(_keyID));
}

bool InputManager::isKeyReleased(int _keyID) {
	return (!isKeyDown(_keyID) && wasKeyDown(_keyID));
}