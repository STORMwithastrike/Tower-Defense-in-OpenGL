#pragma once

#include <GLFW/glfw3.h>

class InputManager {
private:
    static bool keys[1024];
    static bool mouseButtons[8];
    static double mouseX;
    static double mouseY;

public:
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

    static bool isKeyPressed(int key);
    static bool isMouseButtonPressed(int button);
    static double getMouseX();
    static double getMouseY();
};