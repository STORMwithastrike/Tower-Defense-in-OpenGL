#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(float screenWidth, float screenHeight);

    void Update(float dt);

    void SetPosition(const glm::vec2& pos) { position = pos; }
    const glm::vec2& GetPosition() const { return position; }

    void Move(const glm::vec2& delta) { position += delta; }

    void SetZoom(float z);
    float GetZoom() const { return zoom; }

    void SetScreenSize(float width, float height);
    float GetWidth() const { return screenWidth; }
    float GetHeight() const { return screenHeight; }

    // Screen Shake effect (ideal for cutscene gates and heavy combat impacts)
    void TriggerShake(float duration = 0.3f, float magnitude = 5.0f);

    glm::mat4 GetProjectionMatrix() const;
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetViewProjectionMatrix() const;

    // Coordinate conversions (essential for mouse selection and picking)
    glm::vec2 ScreenToWorld(const glm::vec2& screenPos) const;
    glm::vec2 WorldToScreen(const glm::vec2& worldPos) const;

private:
    float screenWidth;
    float screenHeight;
    glm::vec2 position;
    float zoom;

    // Screen shake state
    float shakeTimer;
    float shakeMagnitude;
    glm::vec2 shakeOffset;
};

