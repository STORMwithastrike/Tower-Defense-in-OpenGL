#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

#include "Shader.h"
#include "VertexArray.h"
#include "Camera.h"

class Renderer {
public:
    Renderer(Shader* shader, float screenWidth, float screenHeight);
    ~Renderer();

    void SetCamera(const Camera* camera) { activeCamera = camera; }
    const Camera* GetCamera() const { return activeCamera; }

    void SetScreenSize(float width, float height);

    // Backward-compatible Draw method
    void Draw(const VertexArray& vao, glm::vec2 position, glm::vec2 size,
              float highlight = 0.0f, glm::vec4 tint = glm::vec4(1.0f));

    // High-level 2D shape drawing methods
    void DrawQuad(const glm::vec2& position, const glm::vec2& size,
                  const glm::vec4& color, float highlight = 0.0f,
                  float damageIntensity = 0.0f, float grayscale = 0.0f);

    void DrawTriangle(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3,
                      const glm::vec4& color, float highlight = 0.0f,
                      float damageIntensity = 0.0f);

    void DrawCircle(const glm::vec2& center, float radius,
                    const glm::vec4& color, int segments = 24,
                    float damageIntensity = 0.0f);

    // Specialized game UI elements
    void DrawHealthBar(const glm::vec2& position, const glm::vec2& size,
                       int currentHealth, int maxHealth,
                       bool showBorder = true);

    // Vector font text rendering (supports alphanumeric, punctuation, dialogues & menus)
    void DrawString(const std::string& text, const glm::vec2& position,
                    float scale = 1.0f, const glm::vec4& color = glm::vec4(1.0f));

private:
    Shader* shader;
    const Camera* activeCamera;
    glm::mat4 defaultProjection;
    float screenWidth;
    float screenHeight;

    // Internal dynamic geometry buffers for shapes & text
    GLuint shapeVAO, shapeVBO;

    void initShapeBuffers();
    void drawGlyph(char c, const glm::vec2& pos, float scale, const glm::vec4& color);
};
