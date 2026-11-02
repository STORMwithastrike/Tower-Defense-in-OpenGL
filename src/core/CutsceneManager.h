#pragma once

#include <glm/glm.hpp>
#include "../render/Renderer.h"
#include "../render/Camera.h"
#include "../ui/SubtitleUI.h"

class CutsceneManager {
public:
    CutsceneManager(float screenWidth, float screenHeight);

    void Start();
    void Update(float dt, Camera* camera, bool skipRequested);
    void Render(Renderer& renderer);

    bool IsFinished() const { return finished; }
    void Reset();

private:
    float screenWidth;
    float screenHeight;
    float timer;
    float totalDuration;
    bool finished;

    // Actor positions & anim states
    glm::vec2 princessPos;
    glm::vec2 heroPos;
    glm::vec2 villainPos;

    float heroSwordDraw;
    float villainSlash;
    float clashSparkTimer;

    SubtitleUI subtitleUI;

    void renderPrincess(Renderer& renderer, const glm::vec2& pos);
    void renderHeroKnight(Renderer& renderer, const glm::vec2& pos);
    void renderVillainKnight(Renderer& renderer, const glm::vec2& pos);
    void renderClashSparks(Renderer& renderer, const glm::vec2& pos);
};

