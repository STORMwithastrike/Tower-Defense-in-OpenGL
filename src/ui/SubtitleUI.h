#pragma once

#include <glm/glm.hpp>
#include <string>
#include "../render/Renderer.h"

class SubtitleUI {
public:
    SubtitleUI(float screenWidth, float screenHeight);

    void SetSubtitle(const std::string& speaker, const std::string& line1, const std::string& line2 = "");
    void Clear();

    void Update(float dt);
    void Render(Renderer& renderer);

private:
    float screenWidth;
    float screenHeight;
    std::string currentSpeaker;
    std::string textLine1;
    std::string textLine2;
    bool visible;
    float animTimer;
};

