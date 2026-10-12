#include "../headers/Window.h"
#include "../headers/InputManager.h"
#include <glm/glm.hpp>
#include <iostream>

int main() {
    // Create an 800x600 window to match your team's grid coordinates
    Window gameWindow(800, 600);

    if (!gameWindow.initialize("Castle Defense")) {
        return -1;
    }

    GLFWwindow* rawWindow = gameWindow.getGLFWWindow();
    glfwSetKeyCallback(rawWindow, InputManager::keyCallback);
    glfwSetMouseButtonCallback(rawWindow, InputManager::mouseButtonCallback);
	glfwSetCursorPosCallback(rawWindow, InputManager::cursorPosCallback);

    const glm::vec3 clearColor(0.10f, 0.12f, 0.18f);

    while (!gameWindow.shouldClose()) {
        glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        gameWindow.update();
    }

    return 0;
}