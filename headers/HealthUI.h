#pragma once
#include <glm/glm.hpp>

class Renderer;
class VertexArray;

class HealthUI {
public:
    // Draws a health bar centered above/below an entity using Renderer
    static void draw(Renderer& renderer, 
                     glm::vec2 entityPos, 
                     float entityWidth, 
                     int currentHp, 
                     int maxHp,
                     float yOffset = -15.0f,
                     float barWidth = 34.0f,
                     float barHeight = 5.0f);

    // Overload accepting quadVAO for backward compatibility
    static void draw(Renderer& renderer, 
                     VertexArray& quadVAO, 
                     glm::vec2 entityPos, 
                     float entityWidth, 
                     int currentHp, 
                     int maxHp);
};

