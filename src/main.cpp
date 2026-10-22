#include "../headers/Window.h"
#include "../headers/InputManager.h"
#include "render/Shader.h"
#include "render/VertexArray.h"
#include "render/Renderer.h"
#include "../headers/Grid.h"
#include "../headers/Environment.h"
#include "../headers/Player.h"
#include "../headers/MeleePlayer.h"
#include "../headers/RangedPlayer.h"
#include "../headers/DefensePlayer.h"
#include "../headers/Projectile.h"
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
    Shader myShader(ASSETS_DIR "shaders/basic.vert", ASSETS_DIR "shaders/basic.frag");
    Renderer myRenderer(&myShader, 800.0f, 600.0f);

    // Basic 1x1 quad for highlights
    std::vector<float> quadVertices = {
        0.0f, 0.0f, 0.0f,       1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f,       1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f,       1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,       1.0f, 1.0f, 1.0f
    };
    std::vector<unsigned int> quadIndices = { 0, 1, 2, 2, 3, 0 };
    VertexArray quadVAO(quadVertices, quadIndices);

    // --- 1C & 2C: GRID & ENVIRONMENT SETUP ---
    Grid gameGrid;
    gameGrid.init();
    VertexArray gridVAO(gameGrid.getVertices(), gameGrid.getIndices());

    Environment env;
    env.init();
    VertexArray envVAO(env.getVertices(), env.getIndices());

    LaneSelector laneSelector;

    // --- 2A: PLAYER SETUP ---
    // Melee Knight in Lane 0[cite: 5, 9]
    MeleePlayer meleePlayer(0);
    meleePlayer.init(35.0f, gameGrid.getLaneCenterY(0) - 27.5f);
    VertexArray meleeVAO(meleePlayer.getVertices(), meleePlayer.getIndices());

    // Ranged Archer in Lane 1[cite: 5, 9]
    RangedPlayer rangedPlayer(1);
    rangedPlayer.init(35.0f, gameGrid.getLaneCenterY(1) - 27.5f);
    VertexArray rangedVAO(rangedPlayer.getVertices(), rangedPlayer.getIndices());

    // Defense Guardian in Lane 2[cite: 5, 9]
    DefensePlayer defensePlayer(2);
    defensePlayer.init(35.0f, gameGrid.getLaneCenterY(2) - 27.5f);
    VertexArray defenseVAO(defensePlayer.getVertices(), defensePlayer.getIndices());

    // --- 3A: PROJECTILE SETUP ---
    Projectile protoArrow;
    protoArrow.init(0.0f, 0.0f);
    VertexArray projectileVAO(protoArrow.getVertices(), protoArrow.getIndices());

    std::vector<Projectile> projectiles;
    bool wasSpacePressed = false;

    const glm::vec3 clearColor(0.10f, 0.12f, 0.18f);

    // --- GAME LOOP ---
    while (!gameWindow.shouldClose()) {
        glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 1. Live mouse state
        double mouseX = InputManager::getMouseX();
        double mouseY = InputManager::getMouseY();
        bool isClicking = InputManager::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);

        // 2. Pure-logic LaneSelector
        laneSelector.update(gameGrid, mouseX, mouseY, isClicking);

        // 3. Firing on Spacebar press (from the archer in Lane 1)[cite: 9]
        bool isSpacePressed = InputManager::isKeyPressed(GLFW_KEY_SPACE);
        if (isSpacePressed && !wasSpacePressed) {
            Projectile arrow;
            arrow.init(rangedPlayer.getPosition().x + 35.0f, rangedPlayer.getPosition().y + 24.0f);
            projectiles.push_back(arrow);
        }
        wasSpacePressed = isSpacePressed;

        // 4. Render Grid & Environment
        myRenderer.Draw(gridVAO, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.0f, glm::vec4(1.0f));
        myRenderer.Draw(envVAO, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.0f, glm::vec4(1.0f));

        // 5. Render Highlights
        for (const auto& lane : gameGrid.getLanes()) {
            float highlightAmount = 0.0f;
            glm::vec4 tintColor(1.0f);

            if (lane.id == laneSelector.getSelectedLaneId()) {
                highlightAmount = 1.0f;
                tintColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
            }
            else if (lane.id == laneSelector.getHoveredLaneId()) {
                highlightAmount = 1.0f;
                tintColor = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
            }

            if (highlightAmount > 0.0f) {
                glm::vec2 pos(lane.minX, lane.minY);
                glm::vec2 size(lane.maxX - lane.minX, lane.maxY - lane.minY);
                myRenderer.Draw(quadVAO, pos, size, highlightAmount, tintColor);
            }
        }

        // 6. Render All Three Player Types[cite: 5]
        myRenderer.Draw(meleeVAO, meleePlayer.getRenderPosition(), glm::vec2(1.0f, 1.0f), 0.0f, glm::vec4(1.0f));
        myRenderer.Draw(rangedVAO, rangedPlayer.getRenderPosition(), glm::vec2(1.0f, 1.0f), 0.0f, glm::vec4(1.0f));
        myRenderer.Draw(defenseVAO, defensePlayer.getRenderPosition(), glm::vec2(1.0f, 1.0f), 0.0f, glm::vec4(1.0f));

        // 7. Update & Render Active Projectiles
        for (auto& arrow : projectiles) {
            if (arrow.isActive()) {
                arrow.update();
                myRenderer.Draw(projectileVAO, arrow.getPosition(), glm::vec2(1.0f, 1.0f), 0.0f, glm::vec4(1.0f));
            }
        }

        gameWindow.update();
    }

    return 0;
}