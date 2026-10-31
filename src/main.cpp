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
#include "../headers/ActionManager.h"
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
    const auto& lanes = gameGrid.getLanes();
    float spawnX = lanes[0].minX + 15.0f;

    MeleePlayer meleePlayer(0);
    meleePlayer.init(spawnX, gameGrid.getLaneCenterY(0) - 27.5f);
    VertexArray meleeVAO(meleePlayer.getVertices(), meleePlayer.getIndices());

    RangedPlayer rangedPlayer(1);
    rangedPlayer.init(spawnX, gameGrid.getLaneCenterY(1) - 27.5f);
    VertexArray rangedVAO(rangedPlayer.getVertices(), rangedPlayer.getIndices());

    DefensePlayer defensePlayer(2);
    defensePlayer.init(spawnX, gameGrid.getLaneCenterY(2) - 27.5f);
    VertexArray defenseVAO(defensePlayer.getVertices(), defensePlayer.getIndices());

    std::vector<Player*> activePlayers = { &meleePlayer, &rangedPlayer, &defensePlayer };

    // --- 3A: PROJECTILE SETUP ---
    Projectile protoArrow;
    protoArrow.init(0.0f, 0.0f);
    VertexArray projectileVAO(protoArrow.getVertices(), protoArrow.getIndices());

    std::vector<Projectile> projectiles;
    bool wasSpacePressed = false;

    float lastFrameTime = static_cast<float>(glfwGetTime());
    const glm::vec3 clearColor(0.10f, 0.12f, 0.18f);

    // --- GAME LOOP ---
    while (!gameWindow.shouldClose()) {
        float currentFrameTime = static_cast<float>(glfwGetTime());
        float dt = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 1. Live mouse state
        double mouseX = InputManager::getMouseX();
        double mouseY = InputManager::getMouseY();
        bool isClicking = InputManager::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);

        // 2. Lane selection logic
        laneSelector.update(gameGrid, mouseX, mouseY, isClicking);

        // 3. ActionManager handles W (Jump) and S (Duck) for selected lanes
        std::vector<int> selectedLanes;
        if (laneSelector.getSelectedLaneId() != -1) {
            selectedLanes.push_back(laneSelector.getSelectedLaneId());
        }
        ActionManager::handleInput(activePlayers, selectedLanes);

        // 4. Update Players
        meleePlayer.update(dt);
        rangedPlayer.update(dt);
        defensePlayer.update(dt);

        // 5. Fire projectile on Spacebar tap
        bool isSpacePressed = InputManager::isKeyPressed(GLFW_KEY_SPACE);
        if (isSpacePressed && !wasSpacePressed) {
            Projectile arrow;
            arrow.init(rangedPlayer.getPosition().x + 35.0f, rangedPlayer.getPosition().y + 24.0f);
            projectiles.push_back(arrow);
        }
        wasSpacePressed = isSpacePressed;

        // 6. Render Environment & Grid
        myRenderer.Draw(gridVAO, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.0f, glm::vec4(1.0f));
        myRenderer.Draw(envVAO, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.0f, glm::vec4(1.0f));

        // 7. Render Lane Highlights
        for (const auto& lane : lanes) {
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

        // 8. Render Players
        myRenderer.Draw(meleeVAO, meleePlayer.getRenderPosition(), glm::vec2(1.0f, 1.0f), 0.0f, glm::vec4(1.0f));
        myRenderer.Draw(rangedVAO, rangedPlayer.getRenderPosition(), glm::vec2(1.0f, 1.0f), 0.0f, glm::vec4(1.0f));
        myRenderer.Draw(defenseVAO, defensePlayer.getRenderPosition(), glm::vec2(1.0f, 1.0f), 0.0f, glm::vec4(1.0f));

        // 9. Update & Render Projectiles
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