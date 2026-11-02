#include "Camera.h"
#include <algorithm>
#include <cstdlib>

Camera::Camera(float screenWidth, float screenHeight)
    : screenWidth(screenWidth),
      screenHeight(screenHeight),
      position(0.0f, 0.0f),
      zoom(1.0f),
      shakeTimer(0.0f),
      shakeMagnitude(0.0f),
      shakeOffset(0.0f, 0.0f)
{
}

void Camera::Update(float dt) {
    if (shakeTimer > 0.0f) {
        shakeTimer -= dt;
        if (shakeTimer <= 0.0f) {
            shakeTimer = 0.0f;
            shakeOffset = glm::vec2(0.0f, 0.0f);
        } else {
            // Generate random shake offset in [-shakeMagnitude, +shakeMagnitude]
            float rx = ((float)rand() / (float)RAND_MAX * 2.0f - 1.0f) * shakeMagnitude;
            float ry = ((float)rand() / (float)RAND_MAX * 2.0f - 1.0f) * shakeMagnitude;
            shakeOffset = glm::vec2(rx, ry);
        }
    }
}

void Camera::SetZoom(float z) {
    zoom = std::max(0.1f, std::min(z, 10.0f));
}

void Camera::SetScreenSize(float width, float height) {
    screenWidth = width;
    screenHeight = height;
}

void Camera::TriggerShake(float duration, float magnitude) {
    shakeTimer = duration;
    shakeMagnitude = magnitude;
}

glm::mat4 Camera::GetProjectionMatrix() const {
    // 2D orthographic projection: (0,0) at top-left, (screenWidth, screenHeight) at bottom-right
    return glm::ortho(0.0f, screenWidth, screenHeight, 0.0f, -1.0f, 1.0f);
}

glm::mat4 Camera::GetViewMatrix() const {
    glm::mat4 view = glm::mat4(1.0f);

    // Center zoom on screen center
    glm::vec2 center(screenWidth * 0.5f, screenHeight * 0.5f);
    view = glm::translate(view, glm::vec3(center, 0.0f));
    view = glm::scale(view, glm::vec3(zoom, zoom, 1.0f));
    view = glm::translate(view, glm::vec3(-center, 0.0f));

    // Apply translation + screen shake
    glm::vec2 effectivePos = position + shakeOffset;
    view = glm::translate(view, glm::vec3(-effectivePos.x, -effectivePos.y, 0.0f));

    return view;
}

glm::mat4 Camera::GetViewProjectionMatrix() const {
    return GetProjectionMatrix() * GetViewMatrix();
}

glm::vec2 Camera::ScreenToWorld(const glm::vec2& screenPos) const {
    glm::vec2 center(screenWidth * 0.5f, screenHeight * 0.5f);
    glm::vec2 fromCenter = (screenPos - center) / zoom;
    return fromCenter + center + position;
}

glm::vec2 Camera::WorldToScreen(const glm::vec2& worldPos) const {
    glm::vec2 center(screenWidth * 0.5f, screenHeight * 0.5f);
    glm::vec2 fromCenter = (worldPos - position - center) * zoom;
    return fromCenter + center;
}

