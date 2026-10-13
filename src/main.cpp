#include "../headers/Window.h"
#include "../headers/InputManager.h"

// Changed from "../headers/..." to point to your actual render folder
#include "render/Shader.h"
#include "render/VertexArray.h"
#include "render/Renderer.h"

#include <glm/glm.hpp>
#include <iostream>
#include <vector>

int main() {
    Window gameWindow(800, 600); // creates a 800x600 4:3 aspect ratiowindow

    if (!gameWindow.initialize("Castle Defense")) {
        return -1;
    }

    GLFWwindow* rawWindow = gameWindow.getGLFWWindow();
    glfwSetKeyCallback(rawWindow, InputManager::keyCallback);
    glfwSetMouseButtonCallback(rawWindow, InputManager::mouseButtonCallback);
    glfwSetCursorPosCallback(rawWindow, InputManager::cursorPosCallback);

    // --- 1B: RENDERER SETUP ---
    // 1. Initialize the shaders
    Shader myShader(ASSETS_DIR "shaders/basic.vert", ASSETS_DIR "shaders/basic.frag");

    // 2. Initialize the renderer with the camera dimensions (800x600)
    Renderer myRenderer(&myShader, 800.0f, 600.0f);

    // 3. Define a basic 1x1 square (Top-Left origin layout)
    std::vector<float> quadVertices = {
        // Position (X, Y, Z)   // Color (R, G, B)
        0.0f, 0.0f, 0.0f,       1.0f, 1.0f, 1.0f, // Top-Left (White)
        1.0f, 0.0f, 0.0f,       1.0f, 1.0f, 1.0f, // Top-Right
        1.0f, 1.0f, 0.0f,       1.0f, 1.0f, 1.0f, // Bottom-Right
        0.0f, 1.0f, 0.0f,       1.0f, 1.0f, 1.0f  // Bottom-Left
    };

    std::vector<unsigned int> quadIndices = {
        0, 1, 2, // First triangle
        2, 3, 0  // Second triangle
    };

    // 4. Send the square to the GPU
    VertexArray quadVAO(quadVertices, quadIndices);

    // --- TEST VARIABLES ---
    glm::vec2 testPos(350.0f, 250.0f); // Placed roughly in the center of 800x600
    glm::vec2 testSize(100.0f, 100.0f); // Scaled to 100x100 pixels

    const glm::vec3 clearColor(0.10f, 0.12f, 0.18f);

    // --- GAME LOOP ---
    while (!gameWindow.shouldClose()) {
        glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // --- 1. TEST INPUT (Movement) ---
        float speed = 0.5f;
        if (InputManager::isKeyPressed(GLFW_KEY_W)) testPos.y -= speed;
        if (InputManager::isKeyPressed(GLFW_KEY_S)) testPos.y += speed;
        if (InputManager::isKeyPressed(GLFW_KEY_A)) testPos.x -= speed;
        if (InputManager::isKeyPressed(GLFW_KEY_D)) testPos.x += speed;

        // --- 2. TEST MOUSE (Hover & Highlight Shader) ---
        double mouseX = InputManager::getMouseX();
        double mouseY = InputManager::getMouseY();

        float highlightAmount = 0.0f;
        glm::vec4 tintColor(1.0f, 1.0f, 1.0f, 1.0f); // Default normal color

        // Simple AABB collision to check if mouse is over the square
        if (mouseX >= testPos.x && mouseX <= testPos.x + testSize.x &&
            mouseY >= testPos.y && mouseY <= testPos.y + testSize.y)
        {
            highlightAmount = 1.0f; // Triggers the yellow mix in basic.frag

            // --- 3. TEST CLICKS (Damage Tint Shader) ---
            if (InputManager::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
                tintColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); // Tints it Red when clicked
            }
        }

        // --- 4. TEST RENDERER (Draw Call) ---
        myRenderer.Draw(quadVAO, testPos, testSize, highlightAmount, tintColor);

        gameWindow.update();
    }

    return 0;
}