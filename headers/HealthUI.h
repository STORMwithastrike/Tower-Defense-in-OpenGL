#pragma once
#include <glm/glm.hpp>


class Renderer;
class VertexArray;

class HealthUI {
public:
    // Draws  health bar 
    static void draw(Renderer& renderer, 
                     VertexArray& quadVAO, 
                     glm::vec2 entityPos, 
                     float entityWidth, 
                     int currentHp, 
                     int maxHp);
};

