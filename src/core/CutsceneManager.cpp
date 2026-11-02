#include "CutsceneManager.h"
#include <cmath>

CutsceneManager::CutsceneManager(float screenWidth, float screenHeight)
    : screenWidth(screenWidth),
      screenHeight(screenHeight),
      timer(0.0f),
      totalDuration(14.5f), // 14.5 seconds (within the 10-20s requirement)
      finished(false),
      princessPos(90.0f, 320.0f),
      heroPos(170.0f, 350.0f),
      villainPos(840.0f, 350.0f),
      heroSwordDraw(0.0f),
      villainSlash(0.0f),
      clashSparkTimer(0.0f),
      subtitleUI(screenWidth, screenHeight)
{
}

void CutsceneManager::Start() {
    timer = 0.0f;
    finished = false;
    princessPos = glm::vec2(90.0f, 320.0f);
    heroPos = glm::vec2(170.0f, 350.0f);
    villainPos = glm::vec2(840.0f, 350.0f);
    heroSwordDraw = 0.0f;
    villainSlash = 0.0f;
    clashSparkTimer = 0.0f;
    subtitleUI.Clear();
}

void CutsceneManager::Reset() {
    Start();
}

void CutsceneManager::Update(float dt, Camera* camera, bool skipRequested) {
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
    // Phase 2 (3.8s - 7.8s): Gate Rumbles, Screen Shakes, Malakor Invades
    else if (timer < 7.8f) {
        // Trigger screen shake when gate breaches
        if (timer >= 3.8f && timer < 4.2f && camera) {
            camera->TriggerShake(0.4f, 4.0f);
        }

        // Malakor marches in from right
        if (villainPos.x > 490.0f) {
            villainPos.x -= 85.0f * dt;
        }

        subtitleUI.SetSubtitle("LORD MALAKOR (INVADER)",
                                "SURRENDER THE PRINCESS AND OPEN THE CITADEL GATES,",
                                "OR MY CORRUPT KNIGHTS WILL BURN THIS REALM TO ASHES!");
    }
    // Phase 3 (7.8s - 11.5s): Blue Knight Steps Forward, Draws Sword & Defies
    else if (timer < 11.5f) {
        // Hero advances to confront Malakor
        if (heroPos.x < 370.0f) {
            heroPos.x += 65.0f * dt;
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
        villainSlash = sinf(timer * 6.0f) * 10.0f;

        subtitleUI.SetSubtitle("COMMAND",
                                "DEFEND THE CASTLE CITADEL FROM THE INVADING KNIGHTS!",
                                "WAVE 1 IS APPROACHING!");
    } else {
        finished = true;
        subtitleUI.Clear();
    }
}

void CutsceneManager::Render(Renderer& renderer) {
    // 1. Cinematic Background (Castle Wall & Courtyard Floor)
    renderer.DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(screenWidth, screenHeight),
                      glm::vec4(0.12f, 0.14f, 0.20f, 1.0f));

    // Castle Stone Blocks
    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 12; ++x) {
            float ox = (y % 2 == 1) ? 35.0f : 0.0f;
            renderer.DrawQuad(glm::vec2(x * 70.0f + ox, y * 45.0f + 60.0f), glm::vec2(68.0f, 43.0f),
                              glm::vec4(0.24f, 0.25f, 0.30f, 1.0f));
        }
    }

    // Courtyard Cobblestone Ground
    renderer.DrawQuad(glm::vec2(0.0f, 380.0f), glm::vec2(screenWidth, screenHeight - 380.0f),
                      glm::vec4(0.18f, 0.20f, 0.24f, 1.0f));
    renderer.DrawQuad(glm::vec2(0.0f, 380.0f), glm::vec2(screenWidth, 4.0f),
                      glm::vec4(0.35f, 0.35f, 0.40f, 1.0f)); // Ground line

    // Castle Balcony / Home Base Platform (Left)
    renderer.DrawQuad(glm::vec2(40.0f, 260.0f), glm::vec2(100.0f, 120.0f),
                      glm::vec4(0.35f, 0.36f, 0.42f, 1.0f));
    renderer.DrawQuad(glm::vec2(30.0f, 305.0f), glm::vec2(120.0f, 12.0f),
                      glm::vec4(0.70f, 0.55f, 0.20f, 1.0f)); // Gold balcony trim

    // 2. Render Actors
    renderPrincess(renderer, princessPos);
    renderHeroKnight(renderer, heroPos);
    renderVillainKnight(renderer, villainPos);

    // 3. Render Battle Clash Sparks if in final phase
    if (timer >= 11.5f && timer < totalDuration) {
        renderClashSparks(renderer, (heroPos + villainPos) * 0.5f + glm::vec2(0.0f, -15.0f));
    }

    // 4. Render Subtitle UI
    subtitleUI.Render(renderer);
}

void CutsceneManager::renderPrincess(Renderer& renderer, const glm::vec2& pos) {
    // Composite Princess Model:
    // Flowing Royal Violet Dress
    renderer.DrawQuad(pos + glm::vec2(-10.0f, 2.0f), glm::vec2(20.0f, 34.0f),
                      glm::vec4(0.68f, 0.25f, 0.75f, 1.0f));
    // Gold Corset & Trim
    renderer.DrawQuad(pos + glm::vec2(-8.0f, 0.0f), glm::vec2(16.0f, 12.0f),
                      glm::vec4(0.95f, 0.80f, 0.25f, 1.0f));
    // Head & Face
    renderer.DrawQuad(pos + glm::vec2(-6.0f, -14.0f), glm::vec2(12.0f, 14.0f),
                      glm::vec4(0.98f, 0.85f, 0.75f, 1.0f)); // Skin
    // Golden Blonde Curls
    renderer.DrawQuad(pos + glm::vec2(-9.0f, -16.0f), glm::vec2(4.0f, 20.0f),
                      glm::vec4(0.95f, 0.85f, 0.35f, 1.0f));
    renderer.DrawQuad(pos + glm::vec2(5.0f, -16.0f), glm::vec2(4.0f, 20.0f),
                      glm::vec4(0.95f, 0.85f, 0.35f, 1.0f));
    // Golden Tiara / Crown
    renderer.DrawTriangle(pos + glm::vec2(-6.0f, -16.0f), pos + glm::vec2(6.0f, -16.0f), pos + glm::vec2(0.0f, -24.0f),
                          glm::vec4(1.0f, 0.85f, 0.20f, 1.0f));
    // Clasped hands
    renderer.DrawQuad(pos + glm::vec2(-3.0f, 6.0f), glm::vec2(6.0f, 5.0f),
                      glm::vec4(0.98f, 0.85f, 0.75f, 1.0f));
}

void CutsceneManager::renderHeroKnight(Renderer& renderer, const glm::vec2& pos) {
    // Composite Blue Knight Hero (Sir Gareth):
    // Silver & Royal Blue Armor
    renderer.DrawQuad(pos + glm::vec2(-6.0f, 12.0f), glm::vec2(5.0f, 15.0f), glm::vec4(0.50f, 0.52f, 0.58f, 1.0f));
    renderer.DrawQuad(pos + glm::vec2(2.0f, 12.0f), glm::vec2(5.0f, 15.0f), glm::vec4(0.50f, 0.52f, 0.58f, 1.0f));

    // Royal Blue Surcoat & Torso
    renderer.DrawQuad(pos + glm::vec2(-10.0f, -8.0f), glm::vec2(20.0f, 22.0f),
                      glm::vec4(0.18f, 0.40f, 0.85f, 1.0f)); // Royal Blue
    // Golden Crest
    renderer.DrawQuad(pos + glm::vec2(-4.0f, -3.0f), glm::vec2(8.0f, 10.0f),
                      glm::vec4(0.95f, 0.80f, 0.20f, 1.0f));

    // Silver Bascinet Helmet
    renderer.DrawQuad(pos + glm::vec2(-8.0f, -22.0f), glm::vec2(16.0f, 14.0f),
                      glm::vec4(0.65f, 0.68f, 0.74f, 1.0f));
    renderer.DrawQuad(pos + glm::vec2(-2.0f, -18.0f), glm::vec2(8.0f, 3.0f),
                      glm::vec4(0.10f, 0.12f, 0.18f, 1.0f)); // Eye slit
    // Royal Blue Feather Plume
    renderer.DrawTriangle(pos + glm::vec2(0.0f, -29.0f), pos + glm::vec2(-4.0f, -22.0f), pos + glm::vec2(4.0f, -22.0f),
                          glm::vec4(0.15f, 0.45f, 0.95f, 1.0f));

    // Royal Shield (Front)
    renderer.DrawQuad(pos + glm::vec2(8.0f, -4.0f), glm::vec2(10.0f, 20.0f),
                      glm::vec4(0.15f, 0.35f, 0.80f, 1.0f));
    renderer.DrawTriangle(pos + glm::vec2(8.0f, 16.0f), pos + glm::vec2(18.0f, 16.0f), pos + glm::vec2(13.0f, 24.0f),
                          glm::vec4(0.15f, 0.35f, 0.80f, 1.0f));

    // Shimmering Steel Sword (Drawn upwards in confrontation)
    float drawY = -heroSwordDraw * 26.0f;
    renderer.DrawQuad(pos + glm::vec2(18.0f, 4.0f + drawY), glm::vec2(4.0f, 24.0f),
                      glm::vec4(0.85f, 0.88f, 0.95f, 1.0f));
    renderer.DrawTriangle(pos + glm::vec2(18.0f, 4.0f + drawY),
                          pos + glm::vec2(22.0f, 4.0f + drawY),
                          pos + glm::vec2(20.0f, -4.0f + drawY),
                          glm::vec4(0.95f, 0.95f, 1.0f, 1.0f));
    renderer.DrawQuad(pos + glm::vec2(14.0f, 14.0f + drawY), glm::vec2(12.0f, 3.5f),
                      glm::vec4(0.85f, 0.70f, 0.20f, 1.0f)); // Crossguard
}

void CutsceneManager::renderVillainKnight(Renderer& renderer, const glm::vec2& pos) {
    // Composite Invader Champion (Lord Malakor):
    // Black Iron Plate Greaves & Sabatons
    renderer.DrawQuad(pos + glm::vec2(-8.0f, 12.0f), glm::vec2(6.0f, 16.0f), glm::vec4(0.20f, 0.20f, 0.22f, 1.0f));
    renderer.DrawQuad(pos + glm::vec2(2.0f, 12.0f), glm::vec2(6.0f, 16.0f), glm::vec4(0.20f, 0.20f, 0.22f, 1.0f));

    // Black Breastplate & Blood Red Tabard
    renderer.DrawQuad(pos + glm::vec2(-14.0f, -10.0f), glm::vec2(28.0f, 24.0f),
                      glm::vec4(0.18f, 0.18f, 0.20f, 1.0f));
    renderer.DrawQuad(pos + glm::vec2(-8.0f, -6.0f), glm::vec2(16.0f, 18.0f),
                      glm::vec4(0.65f, 0.10f, 0.10f, 1.0f)); // Blood red tabard

    // Spiked Pauldron
    renderer.DrawTriangle(pos + glm::vec2(-14.0f, -18.0f), pos + glm::vec2(-22.0f, -4.0f), pos + glm::vec2(-10.0f, -4.0f),
                          glm::vec4(0.30f, 0.30f, 0.35f, 1.0f));

    // Horned Great-Helm & Glowing Red Eyes
    renderer.DrawQuad(pos + glm::vec2(-10.0f, -26.0f), glm::vec2(20.0f, 16.0f),
                      glm::vec4(0.25f, 0.25f, 0.28f, 1.0f));
    renderer.DrawQuad(pos + glm::vec2(-10.0f, -20.0f), glm::vec2(14.0f, 3.0f),
                      glm::vec4(1.0f, 0.10f, 0.10f, 1.0f)); // Red glowing eye slit
    // Horns
    renderer.DrawTriangle(pos + glm::vec2(-16.0f, -36.0f), pos + glm::vec2(-10.0f, -26.0f), pos + glm::vec2(-6.0f, -26.0f),
                          glm::vec4(0.15f, 0.15f, 0.16f, 1.0f));
    renderer.DrawTriangle(pos + glm::vec2(16.0f, -36.0f), pos + glm::vec2(6.0f, -26.0f), pos + glm::vec2(10.0f, -26.0f),
                          glm::vec4(0.15f, 0.15f, 0.16f, 1.0f));

    // Executioner Greatsword Pointed Forward
    renderer.DrawQuad(pos + glm::vec2(-44.0f, -4.0f + villainSlash), glm::vec2(32.0f, 8.0f),
                      glm::vec4(0.70f, 0.72f, 0.78f, 1.0f));
    renderer.DrawTriangle(pos + glm::vec2(-54.0f, 0.0f + villainSlash),
                          pos + glm::vec2(-44.0f, -6.0f + villainSlash),
                          pos + glm::vec2(-44.0f, 6.0f + villainSlash),
                          glm::vec4(0.70f, 0.72f, 0.78f, 1.0f));
}

void CutsceneManager::renderClashSparks(Renderer& renderer, const glm::vec2& pos) {
    // Dynamic spark particles flying where swords clash
    float pulse = 0.5f + 0.5f * sinf(clashSparkTimer * 20.0f);
    renderer.DrawCircle(pos, 8.0f * pulse, glm::vec4(1.0f, 0.95f, 0.40f, 1.0f), 10);
    renderer.DrawCircle(pos + glm::vec2(6.0f, -8.0f), 4.0f * pulse, glm::vec4(1.0f, 0.55f, 0.15f, 0.8f), 8);
    renderer.DrawCircle(pos + glm::vec2(-7.0f, -6.0f), 3.5f * pulse, glm::vec4(1.0f, 0.75f, 0.20f, 0.8f), 8);
}

