#include "../headers/InputManager.h"
#include <iostream>

bool InputManager::s_keys[1024] = { false };
bool InputManager::s_mouseButtons[8] = { false };
double InputManager::s_mouseX = 0.0;
double InputManager::s_mouseY = 0.0;

void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			s_keys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			s_keys[key] = false;
		}
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button >= 0 && button < 8) {
		if (action == GLFW_PRESS) {
			s_mouseButtons[button] = true;
		}
		else if (action == GLFW_RELEASE) {
			s_mouseButtons[button] = false;
		}
	}
}

void InputManager::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	s_mouseX = xpos;
	s_mouseY = ypos;
}

bool InputManager::isKeyPressed(int key) {
	if (key >= 0 && key < 1024) {
		return s_keys[key];
	}
	return false;
}

bool InputManager::isMouseButtonPressed(int button) {
	if (button >= 0 && button < 8) {
		return s_mouseButtons[button];
	}
	return false;
}

double InputManager::getMouseX() {
	return s_mouseX;
}

double InputManager::getMouseY() {
	return s_mouseY;
}