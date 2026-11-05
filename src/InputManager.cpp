#include "../headers/InputManager.h"
#include <iostream>

bool InputManager::keys[1024] = { false };
bool InputManager::mouseButtons[8] = { false };
double InputManager::mouseX = 0.0;
double InputManager::mouseY = 0.0;

void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void)window;
    (void)scancode;
    (void)mods;
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            keys[key] = false;
        }
    }
}

void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button >= 0 && button < 8) {
        if (action == GLFW_PRESS) {
            mouseButtons[button] = true;
        }
        else if (action == GLFW_RELEASE) {
            mouseButtons[button] = false;
        }
    }
}

void InputManager::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    mouseX = xpos;
    mouseY = ypos;
}

bool InputManager::isKeyPressed(int key) {
    if (key >= 0 && key < 1024) {
        return keys[key];
    }
    return false;
}

bool InputManager::isMouseButtonPressed(int button) {
    if (button >= 0 && button < 8) {
        return mouseButtons[button];
    }
    return false;
}

double InputManager::getMouseX() {
    return mouseX;
}

double InputManager::getMouseY() {
    return mouseY;
}