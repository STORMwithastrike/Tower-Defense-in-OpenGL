#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "VertexArray.h"

class Renderer {
public:
    // Constructor initializes the camera projection
    Renderer(Shader* shader, float screenWidth, float screenHeight);

    // The main draw function to render any shape on the screen
    void Draw(const VertexArray& vao, glm::vec2 position, glm::vec2 size, float highlight = 0.0f, glm::vec4 tint = glm::vec4(1.0f));

private:
    Shader* shader;
    glm::mat4 projection;
};