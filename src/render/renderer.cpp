#include "Renderer.h"
#include <cmath>
#include <cctype>

Renderer::Renderer(Shader* shader, float screenWidth, float screenHeight)
    : shader(shader),
    activeCamera(nullptr),
    screenWidth(screenWidth),
    screenHeight(screenHeight),
    shapeVAO(0),
    shapeVBO(0)
{
    defaultProjection = glm::ortho(0.0f, screenWidth, screenHeight, 0.0f, -1.0f, 1.0f);
    initShapeBuffers();
}

Renderer::~Renderer() {
    if (shapeVAO != 0) glDeleteVertexArrays(1, &shapeVAO);
    if (shapeVBO != 0) glDeleteBuffers(1, &shapeVBO);
}

void Renderer::SetScreenSize(float width, float height) {
    screenWidth = width;
    screenHeight = height;
    defaultProjection = glm::ortho(0.0f, screenWidth, screenHeight, 0.0f, -1.0f, 1.0f);
}

void Renderer::initShapeBuffers() {
    glGenVertexArrays(1, &shapeVAO);
    glGenBuffers(1, &shapeVBO);

    glBindVertexArray(shapeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, shapeVBO);

    // Dynamic buffer for vertex data: Pos (3 floats) + Color (4 floats) + TexCoords (2 floats) = 9 floats
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9 * 1024, nullptr, GL_DYNAMIC_DRAW);

    // Pos: location 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color: location 1
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // TexCoords: location 2
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::Draw(const VertexArray& vao, glm::vec2 position, glm::vec2 size, float highlight, glm::vec4 tint, float rotation) {
    shader->Use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));

    // Rotate around player center (local mesh coords are centered around x=20, y=0)
    if (rotation != 0.0f) {
        model = glm::translate(model, glm::vec3(20.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-20.0f, 0.0f, 0.0f));
    }

    model = glm::scale(model, glm::vec3(size, 1.0f));

    glm::mat4 vp = activeCamera ? activeCamera->GetViewProjectionMatrix() : defaultProjection;

    // Support both older u_Transform and new u_ViewProjection/u_Model uniforms
    shader->SetMat4("u_Transform", vp * model);
    shader->SetMat4("u_ViewProjection", vp);
    shader->SetMat4("u_Model", model);

    shader->SetFloat("u_Highlight", highlight);
    shader->SetVec4("u_Tint", tint);
    shader->SetFloat("u_DamageIntensity", 0.0f);
    shader->SetFloat("u_Grayscale", 0.0f);
    shader->SetInt("u_UseTexture", 0);

    vao.Bind();
    glDrawElements(GL_TRIANGLES, vao.GetIndexCount(), GL_UNSIGNED_INT, 0);
    vao.Unbind();
}

void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size,
    const glm::vec4& color, float highlight,
    float damageIntensity, float grayscale)
{
    shader->Use();

    glm::mat4 vp = activeCamera ? activeCamera->GetViewProjectionMatrix() : defaultProjection;
    glm::mat4 model = glm::mat4(1.0f);

    shader->SetMat4("u_Transform", vp);
    shader->SetMat4("u_ViewProjection", vp);
    shader->SetMat4("u_Model", model);
    shader->SetFloat("u_Highlight", highlight);
    shader->SetFloat("u_DamageIntensity", damageIntensity);
    shader->SetFloat("u_Grayscale", grayscale);
    shader->SetVec4("u_Tint", glm::vec4(1.0f));
    shader->SetInt("u_UseTexture", 0);

    float x0 = position.x;
    float y0 = position.y;
    float x1 = position.x + size.x;
    float y1 = position.y + size.y;

    float r = color.r, g = color.g, b = color.b, a = color.a;

    // 2 triangles (6 vertices)
    float vertices[] = {
        // x,   y,   z,      r, g, b, a,     u, v
        x0, y0, 0.0f,   r, g, b, a,     0.0f, 0.0f,
        x1, y0, 0.0f,   r, g, b, a,     1.0f, 0.0f,
        x1, y1, 0.0f,   r, g, b, a,     1.0f, 1.0f,

        x0, y0, 0.0f,   r, g, b, a,     0.0f, 0.0f,
        x1, y1, 0.0f,   r, g, b, a,     1.0f, 1.0f,
        x0, y1, 0.0f,   r, g, b, a,     0.0f, 1.0f
    };

    glBindVertexArray(shapeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, shapeVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::DrawTriangle(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3,
    const glm::vec4& color, float highlight,
    float damageIntensity)
{
    shader->Use();

    glm::mat4 vp = activeCamera ? activeCamera->GetViewProjectionMatrix() : defaultProjection;

    shader->SetMat4("u_Transform", vp);
    shader->SetMat4("u_ViewProjection", vp);
    shader->SetMat4("u_Model", glm::mat4(1.0f));
    shader->SetFloat("u_Highlight", highlight);
    shader->SetFloat("u_DamageIntensity", damageIntensity);
    shader->SetFloat("u_Grayscale", 0.0f);
    shader->SetVec4("u_Tint", glm::vec4(1.0f));
    shader->SetInt("u_UseTexture", 0);

    float r = color.r, g = color.g, b = color.b, a = color.a;

    float vertices[] = {
        p1.x, p1.y, 0.0f,  r, g, b, a,  0.0f, 0.0f,
        p2.x, p2.y, 0.0f,  r, g, b, a,  0.5f, 1.0f,
        p3.x, p3.y, 0.0f,  r, g, b, a,  1.0f, 0.0f
    };

    glBindVertexArray(shapeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, shapeVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::DrawCircle(const glm::vec2& center, float radius,
    const glm::vec4& color, int segments,
    float damageIntensity)
{
    if (segments < 3) segments = 3;
    if (segments > 64) segments = 64;

    shader->Use();

    glm::mat4 vp = activeCamera ? activeCamera->GetViewProjectionMatrix() : defaultProjection;
    shader->SetMat4("u_Transform", vp);
    shader->SetMat4("u_ViewProjection", vp);
    shader->SetMat4("u_Model", glm::mat4(1.0f));
    shader->SetFloat("u_Highlight", 0.0f);
    shader->SetFloat("u_DamageIntensity", damageIntensity);
    shader->SetFloat("u_Grayscale", 0.0f);
    shader->SetVec4("u_Tint", glm::vec4(1.0f));
    shader->SetInt("u_UseTexture", 0);

    std::vector<float> verts;
    verts.reserve((segments + 2) * 9);

    float r = color.r, g = color.g, b = color.b, a = color.a;

    // Center vertex
    verts.push_back(center.x);
    verts.push_back(center.y);
    verts.push_back(0.0f);
    verts.push_back(r); verts.push_back(g); verts.push_back(b); verts.push_back(a);
    verts.push_back(0.5f); verts.push_back(0.5f);

    const float twoPi = 6.28318530718f;
    for (int i = 0; i <= segments; ++i) {
        float theta = i * twoPi / segments;
        float x = center.x + radius * cosf(theta);
        float y = center.y + radius * sinf(theta);

        verts.push_back(x);
        verts.push_back(y);
        verts.push_back(0.0f);
        verts.push_back(r); verts.push_back(g); verts.push_back(b); verts.push_back(a);
        verts.push_back(0.5f + 0.5f * cosf(theta));
        verts.push_back(0.5f + 0.5f * sinf(theta));
    }

    glBindVertexArray(shapeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, shapeVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, verts.size() * sizeof(float), verts.data());

    glDrawArrays(GL_TRIANGLE_FAN, 0, segments + 2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::DrawHealthBar(const glm::vec2& position, const glm::vec2& size,
    int currentHealth, int maxHealth,
    bool showBorder)
{
    if (maxHealth <= 0) return;
    float pct = std::max(0.0f, std::min(1.0f, (float)currentHealth / (float)maxHealth));

    // Outer border / background
    if (showBorder) {
        DrawQuad(position - glm::vec2(1.0f, 1.0f), size + glm::vec2(2.0f, 2.0f),
            glm::vec4(0.05f, 0.05f, 0.05f, 0.9f));
    }
    // Bar background (depleted health)
    DrawQuad(position, size, glm::vec4(0.25f, 0.10f, 0.10f, 0.85f));

    // Foreground color shifts from Green -> Yellow -> Red
    glm::vec4 barColor;
    if (pct > 0.6f) {
        barColor = glm::vec4(0.2f, 0.85f, 0.25f, 1.0f); // Green
    }
    else if (pct > 0.3f) {
        barColor = glm::vec4(0.95f, 0.85f, 0.2f, 1.0f); // Yellow/Orange
    }
    else {
        barColor = glm::vec4(0.95f, 0.2f, 0.2f, 1.0f);  // Critical Red
    }

    glm::vec2 filledSize(size.x * pct, size.y);
    DrawQuad(position, filledSize, barColor);
}

// Crisp Vector Stroke Font Renderer for dialogue, menus, and UI numbers
void Renderer::drawGlyph(char c, const glm::vec2& pos, float scale, const glm::vec4& color) {
    char ch = std::toupper(c);
    float w = 8.0f * scale;
    float h = 12.0f * scale;
    float t = 1.6f * scale; // stroke thickness

    auto stroke = [&](float x, float y, float sw, float sh) {
        DrawQuad(pos + glm::vec2(x * scale, y * scale),
            glm::vec2(sw * scale, sh * scale), color);
        };

    switch (ch) {
    case 'A':
        stroke(0, 0, 1.6f, 12); stroke(6.4f, 0, 1.6f, 12);
        stroke(0, 0, 8, 1.6f);  stroke(0, 6, 8, 1.6f);
        break;
    case 'B':
        stroke(0, 0, 1.6f, 12); stroke(0, 0, 6, 1.6f);
        stroke(0, 5.2f, 6, 1.6f); stroke(0, 10.4f, 6, 1.6f);
        stroke(6, 0, 1.6f, 5.5f); stroke(6, 5.5f, 1.6f, 6.5f);
        break;
    case 'C':
        stroke(0, 0, 1.6f, 12); stroke(0, 0, 8, 1.6f); stroke(0, 10.4f, 8, 1.6f);
        break;
    case 'D':
        stroke(0, 0, 1.6f, 12); stroke(0, 0, 6, 1.6f); stroke(0, 10.4f, 6, 1.6f);
        stroke(6, 1.6f, 1.6f, 8.8f);
        break;
    case 'E':
        stroke(0, 0, 1.6f, 12); stroke(0, 0, 8, 1.6f);
        stroke(0, 5.2f, 6, 1.6f); stroke(0, 10.4f, 8, 1.6f);
        break;
    case 'F':
        stroke(0, 0, 1.6f, 12); stroke(0, 0, 8, 1.6f); stroke(0, 5.2f, 5.5f, 1.6f);
        break;
    case 'G':
        stroke(0, 0, 1.6f, 12); stroke(0, 0, 8, 1.6f); stroke(0, 10.4f, 8, 1.6f);
        stroke(6.4f, 5.2f, 1.6f, 6.8f); stroke(3.5f, 5.2f, 4.5f, 1.6f);
        break;
    case 'H':
        stroke(0, 0, 1.6f, 12); stroke(6.4f, 0, 1.6f, 12); stroke(0, 5.2f, 8, 1.6f);
        break;
    case 'I':
        stroke(3.2f, 0, 1.6f, 12); stroke(0, 0, 8, 1.6f); stroke(0, 10.4f, 8, 1.6f);
        break;
    case 'J':
        stroke(6.4f, 0, 1.6f, 10.4f); stroke(0, 10.4f, 8, 1.6f); stroke(0, 6.0f, 1.6f, 6.0f);
        break;
    case 'K':
        stroke(0, 0, 1.6f, 12); stroke(2, 5.2f, 4.5f, 1.6f);
        stroke(5, 0, 1.6f, 6.0f); stroke(5, 6.0f, 1.6f, 6.0f);
        break;
    case 'L':
        stroke(0, 0, 1.6f, 12); stroke(0, 10.4f, 8, 1.6f);
        break;
    case 'M':
        stroke(0, 0, 1.6f, 12); stroke(6.4f, 0, 1.6f, 12);
        stroke(2, 2, 2, 3); stroke(4, 2, 2, 3);
        break;
    case 'N':
        stroke(0, 0, 1.6f, 12); stroke(6.4f, 0, 1.6f, 12);
        stroke(2.5f, 3, 3, 3);
        break;
    case 'O':
        stroke(0, 0, 1.6f, 12); stroke(6.4f, 0, 1.6f, 12);
        stroke(0, 0, 8, 1.6f); stroke(0, 10.4f, 8, 1.6f);
        break;
    case 'P':
        stroke(0, 0, 1.6f, 12); stroke(0, 0, 8, 1.6f);
        stroke(6.4f, 0, 1.6f, 6.0f); stroke(0, 5.2f, 8, 1.6f);
        break;
    case 'Q':
        stroke(0, 0, 1.6f, 12); stroke(6.4f, 0, 1.6f, 10.4f);
        stroke(0, 0, 8, 1.6f); stroke(0, 10.4f, 8, 1.6f);
        stroke(4.5f, 8.5f, 3.5f, 3.5f);
        break;
    case 'R':
        stroke(0, 0, 1.6f, 12); stroke(0, 0, 8, 1.6f);
        stroke(6.4f, 0, 1.6f, 6.0f); stroke(0, 5.2f, 8, 1.6f);
        stroke(5.0f, 6.0f, 2.0f, 6.0f);
        break;
    case 'S':
        stroke(0, 0, 8, 1.6f); stroke(0, 0, 1.6f, 6.0f);
        stroke(0, 5.2f, 8, 1.6f); stroke(6.4f, 5.2f, 1.6f, 6.8f);
        stroke(0, 10.4f, 8, 1.6f);
        break;
    case 'T':
        stroke(0, 0, 8, 1.6f); stroke(3.2f, 0, 1.6f, 12);
        break;
    case 'U':
        stroke(0, 0, 1.6f, 10.4f); stroke(6.4f, 0, 1.6f, 10.4f);
        stroke(0, 10.4f, 8, 1.6f);
        break;
    case 'V':
        stroke(0, 0, 1.6f, 8); stroke(6.4f, 0, 1.6f, 8);
        stroke(3.2f, 8, 1.6f, 4);
        break;
    case 'W':
        stroke(0, 0, 1.6f, 12); stroke(6.4f, 0, 1.6f, 12);
        stroke(3.2f, 4, 1.6f, 8);
        break;
    case 'X':
        stroke(1, 1, 2, 2); stroke(5, 1, 2, 2);
        stroke(3, 4.5f, 2, 3);
        stroke(1, 9, 2, 2); stroke(5, 9, 2, 2);
        break;
    case 'Y':
        stroke(0, 0, 1.6f, 5); stroke(6.4f, 0, 1.6f, 5);
        stroke(0, 4.5f, 8, 1.6f); stroke(3.2f, 5.5f, 1.6f, 6.5f);
        break;
    case 'Z':
        stroke(0, 0, 8, 1.6f); stroke(0, 10.4f, 8, 1.6f);
        stroke(5, 2, 2, 3); stroke(3, 5, 2, 3); stroke(1, 8, 2, 3);
        break;
    case '0':
        stroke(0, 0, 1.6f, 12); stroke(6.4f, 0, 1.6f, 12);
        stroke(0, 0, 8, 1.6f); stroke(0, 10.4f, 8, 1.6f);
        break;
    case '1':
        stroke(3.2f, 0, 1.6f, 12); stroke(1.5f, 0, 2.5f, 1.6f); stroke(0, 10.4f, 8, 1.6f);
        break;
    case '2':
        stroke(0, 0, 8, 1.6f); stroke(6.4f, 0, 1.6f, 6);
        stroke(0, 5.2f, 8, 1.6f); stroke(0, 6, 1.6f, 6);
        stroke(0, 10.4f, 8, 1.6f);
        break;
    case '3':
        stroke(0, 0, 8, 1.6f); stroke(0, 5.2f, 8, 1.6f); stroke(0, 10.4f, 8, 1.6f);
        stroke(6.4f, 0, 1.6f, 12);
        break;
    case '4':
        stroke(0, 0, 1.6f, 6); stroke(0, 5.2f, 8, 1.6f); stroke(6.4f, 0, 1.6f, 12);
        break;
    case '5':
        stroke(0, 0, 8, 1.6f); stroke(0, 0, 1.6f, 6);
        stroke(0, 5.2f, 8, 1.6f); stroke(6.4f, 5.2f, 1.6f, 6.8f);
        stroke(0, 10.4f, 8, 1.6f);
        break;
    case '6':
        stroke(0, 0, 1.6f, 12); stroke(0, 0, 8, 1.6f);
        stroke(0, 5.2f, 8, 1.6f); stroke(0, 10.4f, 8, 1.6f);
        stroke(6.4f, 5.2f, 1.6f, 6.8f);
        break;
    case '7':
        stroke(0, 0, 8, 1.6f); stroke(6.4f, 0, 1.6f, 12);
        break;
    case '8':
        stroke(0, 0, 1.6f, 12); stroke(6.4f, 0, 1.6f, 12);
        stroke(0, 0, 8, 1.6f); stroke(0, 5.2f, 8, 1.6f); stroke(0, 10.4f, 8, 1.6f);
        break;
    case '9':
        stroke(0, 0, 1.6f, 6); stroke(6.4f, 0, 1.6f, 12);
        stroke(0, 0, 8, 1.6f); stroke(0, 5.2f, 8, 1.6f);
        break;
    case ':':
        stroke(3.2f, 3, 1.8f, 1.8f); stroke(3.2f, 8, 1.8f, 1.8f);
        break;
    case '.':
        stroke(3.2f, 10.2f, 1.8f, 1.8f);
        break;
    case '!':
        stroke(3.2f, 0, 1.8f, 7.5f); stroke(3.2f, 10.2f, 1.8f, 1.8f);
        break;
    case '?':
        stroke(0, 0, 8, 1.6f); stroke(6.4f, 0, 1.6f, 5);
        stroke(2, 4.5f, 5, 1.6f); stroke(3.2f, 5.5f, 1.6f, 2.5f);
        stroke(3.2f, 10.2f, 1.8f, 1.8f);
        break;
    case '-':
        stroke(1.0f, 5.5f, 6.0f, 1.6f);
        break;
    case '[':
        stroke(1.0f, 0, 1.6f, 12); stroke(1.0f, 0, 5, 1.6f); stroke(1.0f, 10.4f, 5, 1.6f);
        break;
    case ']':
        stroke(5.4f, 0, 1.6f, 12); stroke(1.0f, 0, 5, 1.6f); stroke(1.0f, 10.4f, 5, 1.6f);
        break;
    case '/':
        stroke(5.0f, 0, 1.6f, 4); stroke(3.2f, 4, 1.6f, 4); stroke(1.4f, 8, 1.6f, 4);
        break;
    case ' ':
    default:
        break;
    }
}

void Renderer::DrawString(const std::string& text, const glm::vec2& position,
    float scale, const glm::vec4& color)
{
    float advance = 11.0f * scale;
    glm::vec2 currentPos = position;

    for (char c : text) {
        if (c == '\n') {
            currentPos.x = position.x;
            currentPos.y += 18.0f * scale;
            continue;
        }
        drawGlyph(c, currentPos, scale, color);
        currentPos.x += advance;
    }
}