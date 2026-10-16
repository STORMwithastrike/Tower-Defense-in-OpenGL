#include "../headers/Window.h"
#include "../headers/InputManager.h"
#include "render/Shader.h"
#include "render/VertexArray.h"
#include "render/Renderer.h"
#include "../headers/Grid.h"
#include "../headers/LaneSelector.h"

#include <glm/glm.hpp>
#include <iostream>
#include <vector>

int main() {
    Window gameWindow(800, 600);

    if (!gameWindow.initialize("Castle Defense")) {
        return -1;
    }

    GLFWwindow* rawWindow = gameWindow.getGLFWWindow();
    glfwSetKeyCallback(rawWindow, InputManager::keyCallback);
    glfwSetMouseButtonCallback(rawWindow, InputManager::mouseButtonCallback);
    glfwSetCursorPosCallback(rawWindow, InputManager::cursorPosCallback);

    // --- 1B: RENDERER SETUP ---
    // Using the fixed relative paths so the screen isn't black
    Shader myShader(ASSETS_DIR "shaders/basic.vert", ASSETS_DIR "shaders/basic.frag");
    Renderer myRenderer(&myShader, 800.0f, 600.0f);

    // Define a basic 1x1 square (Top-Left origin layout) to use as our highlight overlay
    std::vector<float> quadVertices = {
        0.0f, 0.0f, 0.0f,       1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f,       1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f,       1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,       1.0f, 1.0f, 1.0f
    };
    std::vector<unsigned int> quadIndices = { 0, 1, 2, 2, 3, 0 };
    VertexArray quadVAO(quadVertices, quadIndices);

    // --- 1C & 2C: GRID AND LOGIC SETUP ---
    Grid gameGrid;
    gameGrid.init(); // Tells your teammate's code to generate the lane coordinates

    LaneSelector laneSelector;

    const glm::vec3 clearColor(0.10f, 0.12f, 0.18f);

    // --- GAME LOOP ---
    while (!gameWindow.shouldClose()) {
        glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 1. Ask InputManager for the live mouse state
        double mouseX = InputManager::getMouseX();
        double mouseY = InputManager::getMouseY();
        bool isClicking = InputManager::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);

        // 2. Feed the grid and inputs into your pure-logic LaneSelector
        laneSelector.update(gameGrid, mouseX, mouseY, isClicking);

        // 3. Render the highlights
        for (const auto& lane : gameGrid.getLanes()) {
            float highlightAmount = 0.0f;
            glm::vec4 tintColor(1.0f); // Default invisible

            // Check your LaneSelector's memory to see if this lane needs a highlight
            if (lane.id == laneSelector.getSelectedLaneId()) {
                highlightAmount = 1.0f;
                tintColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); // Red if locked
            }
            else if (lane.id == laneSelector.getHoveredLaneId()) {
                highlightAmount = 1.0f;
                tintColor = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f); // Yellow if hovered
            }

            // If the lane is interacted with, use the teammate's Renderer to draw the 1x1 quad scaled up
            if (highlightAmount > 0.0f) {
                glm::vec2 pos(lane.minX, lane.minY);
                glm::vec2 size(lane.maxX - lane.minX, lane.maxY - lane.minY);
                myRenderer.Draw(quadVAO, pos, size, highlightAmount, tintColor);
            }
        }

        gameWindow.update();
    }

    return 0;
}