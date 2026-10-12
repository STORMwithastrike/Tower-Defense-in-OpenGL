#pragma once
#include <GLFW/glfw3.h>

class InputManager {
private:
	static bool s_keys[1024];
	static bool s_mouseButtons[8];

	static double s_mouseX;
	static double s_mouseY;

public:
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

	static bool isKeyPressed(int key);
	static bool isMouseButtonPressed(int button);
	static double getMouseX();
	static double getMouseY();
};