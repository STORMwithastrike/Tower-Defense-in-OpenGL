#include "../headers/InputManager.h"
#include <iostream>

// bn-initialize el static arrays wel coordinates b zeros w false
bool InputManager::keys[1024] = { false };
bool InputManager::mouseButtons[8] = { false };
double InputManager::mouseX = 0.0;
double InputManager::mouseY = 0.0;

// callback btetnadah lama ay zorrar f el keyboard yetdas aw yetseb
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

// callback bta3et clickat el mouse
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

// callback btetnadah kol ma el mouse yeta7arak 3ashan n-update makan el cursor
void InputManager::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    mouseX = xpos;
    mouseY = ypos;
}

// check law zorrar f el keyboard madoos 3aleeh delwa2ty
bool InputManager::isKeyPressed(int key) {
    if (key >= 0 && key < 1024) {
        return keys[key];
    }
    return false;
}

// check law clicket el mouse madoosa
bool InputManager::isMouseButtonPressed(int button) {
    if (button >= 0 && button < 8) {
        return mouseButtons[button];
    }
    return false;
}

// getters le makan el mouse 3ala el shasha
double InputManager::getMouseX() {
    return mouseX;
}

double InputManager::getMouseY() {
    return mouseY;
}