#include "CutsceneManager.h"
#include <cmath>

CutsceneManager::CutsceneManager(float screenWidth, float screenHeight)
    : screenWidth(screenWidth),
      screenHeight(screenHeight),
      timer(0.0f),
      totalDuration(14.5f), // 14.5 seconds (within the 10-20s requirement)
      finished(false),
      princessPos(50.0f, 191.0f),
      heroPos(160.0f, 375.0f),
      villainPos(760.0f, 375.0f),
      heroSwordDraw(0.0f),
      villainSlash(0.0f),
      clashSparkTimer(0.0f),
      subtitleUI(screenWidth, screenHeight)
{
}

void CutsceneManager::Start() {
    timer = 0.0f;
    finished = false;
    princessPos = glm::vec2(50.0f, 191.0f);
    heroPos = glm::vec2(160.0f, 375.0f);
    villainPos = glm::vec2(760.0f, 375.0f);
    heroSwordDraw = 0.0f;
    villainSlash = 0.0f;
    clashSparkTimer = 0.0f;
    subtitleUI.Clear();
}

void CutsceneManager::Reset() {
    Start();
}

void CutsceneManager::Update(float dt, Camera* camera, bool skipRequested) {
    (void)camera; // Camera effects simplified to maintain consistent gameplay view
    if (finished) return;

    if (skipRequested) {
        finished = true;
        subtitleUI.Clear();
        return;
    }

    timer += dt;
    subtitleUI.Update(dt);

    // Timeline Events:
    // Phase 1 (0.0s - 3.8s): Princess & Blue Knight at Citadel
    if (timer < 3.8f) {
        subtitleUI.SetSubtitle("SIR GARETH (BLUE KNIGHT)",
                                "FEAR NOT, PRINCESS AURELIA.",
                                "THE ROYAL GUARD WILL DEFEND THESE GATES WITH OUR LIVES!");
    }
    // Phase 2 (3.8s - 7.8s): Invader appears on the right
    else if (timer < 7.8f) {
        // Enemy marches in along the lane from right
        if (villainPos.x > 460.0f) {
            villainPos.x -= 80.0f * dt;
        }

        subtitleUI.SetSubtitle("LORD MALAKOR (INVADER)",
                                "SURRENDER THE PRINCESS AND OPEN THE CITADEL GATES,",
                                "OR MY KNIGHTS WILL BREAK THROUGH YOUR DEFENSES!");
    }
    // Phase 3 (7.8s - 11.5s): Blue Knight Steps Forward, Draws Sword & Defies
    else if (timer < 11.5f) {
        // Hero advances to confront Malakor
        if (heroPos.x < 360.0f) {
            heroPos.x += 60.0f * dt;
        }
        if (heroSwordDraw < 1.0f) {
            heroSwordDraw += dt * 1.5f;
            if (heroSwordDraw > 1.0f) heroSwordDraw = 1.0f;
        }

        subtitleUI.SetSubtitle("SIR GARETH (BLUE KNIGHT)",
                                "THE ROYAL GUARD NEVER YIELDS TO TRAITORS!",
                                "ARCHERS, WIZARDS, KNIGHTS -- TO ARMS! DEFEND THE PRINCESS!");
    }
    // Phase 4 (11.5s - 14.5s): Battle Stance & Clash Sparks
    else if (timer < totalDuration) {
        clashSparkTimer += dt;
        villainSlash = sinf(timer * 6.0f) * 6.0f;

        subtitleUI.SetSubtitle("COMMAND",
                                "DEFEND THE CASTLE CITADEL FROM THE INVADING KNIGHTS!",
                                "WAVE 1 IS APPROACHING!");
    } else {
        finished = true;
        subtitleUI.Clear();
    }
}

void CutsceneManager::Render(Renderer& renderer) {
    // Cutscene background is rendered as the gameplay field directly in main.cpp.
    // Here we render the actors and UI on top of the field.

    // 1. Render Actors (matching gameplay models)
    renderHeroKnight(renderer, heroPos);
    renderVillainKnight(renderer, villainPos);

    // 2. Render Battle Clash Sparks if in final phase
    if (timer >= 11.5f && timer < totalDuration) {
        renderClashSparks(renderer, (heroPos + villainPos) * 0.5f + glm::vec2(0.0f, -5.0f));
    }

    // 3. Render Simplified Subtitle UI
    subtitleUI.Render(renderer);
}

void CutsceneManager::renderPrincess(Renderer& renderer, const glm::vec2& pos) {
    (void)renderer;
    (void)pos;
    // Princess is rendered on the castle balcony as part of the gameplay field
}

void CutsceneManager::renderHeroKnight(Renderer& renderer, const glm::vec2& pos) {
    // Exact gameplay MeleePlayer model (Head, Blue body, Sword)
    // 1. Head
    renderer.DrawQuad(pos + glm::vec2(10.0f, -19.0f), glm::vec2(16.0f, 14.0f),
                      glm::vec4(0.60f, 0.65f, 0.70f, 1.0f));
    // 2. Body (Blue clothing)
    renderer.DrawQuad(pos + glm::vec2(8.0f, -5.0f), glm::vec2(20.0f, 24.0f),
                      glm::vec4(0.35f, 0.45f, 0.60f, 1.0f));
    // 3. Sword (raised/drawn)
    float drawAnim = -heroSwordDraw * 8.0f;
    renderer.DrawQuad(pos + glm::vec2(28.0f, 3.0f + drawAnim), glm::vec2(14.0f, 4.0f),
                      glm::vec4(0.85f, 0.85f, 0.90f, 1.0f));
    renderer.DrawTriangle(pos + glm::vec2(42.0f, 1.0f + drawAnim),
                          pos + glm::vec2(42.0f, 9.0f + drawAnim),
                          pos + glm::vec2(48.0f, 5.0f + drawAnim),
                          glm::vec4(0.85f, 0.85f, 0.90f, 1.0f));
}

void CutsceneManager::renderVillainKnight(Renderer& renderer, const glm::vec2& pos) {
    // Exact gameplay Enemy model (matches player geometry, facing left with Red clothing)
    // 1. Head
    renderer.DrawQuad(pos + glm::vec2(-26.0f, -19.0f), glm::vec2(16.0f, 14.0f),
                      glm::vec4(0.60f, 0.65f, 0.70f, 1.0f));
    // 2. Body (Enemy Red clothing)
    renderer.DrawQuad(pos + glm::vec2(-28.0f, -5.0f), glm::vec2(20.0f, 24.0f),
                      glm::vec4(0.75f, 0.15f, 0.15f, 1.0f));
    // 3. Sword (pointing left towards hero)
    renderer.DrawQuad(pos + glm::vec2(-42.0f, 3.0f + villainSlash), glm::vec2(14.0f, 4.0f),
                      glm::vec4(0.85f, 0.85f, 0.90f, 1.0f));
    renderer.DrawTriangle(pos + glm::vec2(-42.0f, 1.0f + villainSlash),
                          pos + glm::vec2(-42.0f, 9.0f + villainSlash),
                          pos + glm::vec2(-48.0f, 5.0f + villainSlash),
                          glm::vec4(0.85f, 0.85f, 0.90f, 1.0f));
}

void CutsceneManager::renderClashSparks(Renderer& renderer, const glm::vec2& pos) {
    float pulse = 0.5f + 0.5f * sinf(clashSparkTimer * 15.0f);
    renderer.DrawQuad(pos + glm::vec2(-3.0f * pulse, -3.0f * pulse),
                      glm::vec2(6.0f * pulse, 6.0f * pulse),
                      glm::vec4(1.0f, 0.90f, 0.30f, 1.0f));
    renderer.DrawQuad(pos + glm::vec2(4.0f, -6.0f), glm::vec2(3.0f, 3.0f),
                      glm::vec4(1.0f, 0.60f, 0.20f, 0.9f));
    renderer.DrawQuad(pos + glm::vec2(-5.0f, -4.0f), glm::vec2(3.0f, 3.0f),
                      glm::vec4(1.0f, 0.80f, 0.20f, 0.9f));
}

