#include "Renderer.h"

Renderer::Renderer(Shader* shader, float screenWidth, float screenHeight) {
    this->shader = shader;

    // Set up Orthographic Projection (0,0 is at the top-left corner)
    this->projection = glm::ortho(0.0f, screenWidth, screenHeight, 0.0f, -1.0f, 1.0f);
}

void Renderer::Draw(const VertexArray& vao, glm::vec2 position, glm::vec2 size, float highlight, glm::vec4 tint) {
    // 1. Activate the shader program
    shader->Use();

    // 2. Initialize an Identity Matrix
    glm::mat4 model = glm::mat4(1.0f);

    // 3. Translate the shape to its X, Y position
    // We translate first, then scale (matrix multiplication happens in reverse order in GLM)
    model = glm::translate(model, glm::vec3(position, 0.0f));

    // 4. Scale the shape to its width and height
    model = glm::scale(model, glm::vec3(size, 1.0f));

    // 5. Combine the Camera (Projection) with the Shape's Position (Model)
    glm::mat4 finalTransform = this->projection * model;

    // 6. Send the uniforms to the GPU using the exact names from basic.vert and basic.frag
    shader->SetMat4("u_Transform", finalTransform);
    shader->SetFloat("u_Highlight", highlight);
    shader->SetVec4("u_Tint", tint.r, tint.g, tint.b, tint.a);

    // 7. Bind the VAO
    vao.Bind();

    // 8. Execute the Draw Command using Element Buffer Objects
    glDrawElements(GL_TRIANGLES, vao.GetIndexCount(), GL_UNSIGNED_INT, 0);

    // 9. Unbind the VAO to keep the state clean
    vao.Unbind();
}