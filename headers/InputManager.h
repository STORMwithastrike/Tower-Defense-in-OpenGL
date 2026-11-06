#pragma once

#include <GLFW/glfw3.h>

// static class mas2oola 3an tracking el keyboard wel mouse inputs
class InputManager {
private:
    static bool keys[1024];       // array bte7fez 7alet kol zorrar f el keyboard
    static bool mouseButtons[8];  // zrayer el mouse (left, right, scroll click)
    static double mouseX;         // makan cursor el mouse X
    static double mouseY;         // makan cursor el mouse Y

public:
    // glfw callbacks 3ashan y-listen 3ala ay input event
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

    // static checkers 3ashan nestakhdemhom f ay makan mn gher object
    static bool isKeyPressed(int key);
    static bool isMouseButtonPressed(int button);

    // getters le makan el mouse el 7aly
    static double getMouseX();
    static double getMouseY();
};