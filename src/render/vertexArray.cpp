#include "VertexArray.h"

VertexArray::VertexArray(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
    indexCount = static_cast<unsigned int>(indices.size());

    // 1. Generate the buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // 2. Bind the VAO first
    glBindVertexArray(VAO);

    // 3. Bind VBO and send the vertex data to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // 4. Bind EBO and send the index data to the GPU
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // 5. Configure the Vertex Attributes (Based on Lab's 6-float stride)

    // Attribute 0: Position (3 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Attribute 1: Color (3 floats)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 6. Unbind the VAO safely
    glBindVertexArray(0);
    // Unbind VBO after VAO is unbound (but do NOT unbind the EBO while the VAO is active!)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void VertexArray::Bind() const {
    glBindVertexArray(VAO);
}

void VertexArray::Unbind() const {
    glBindVertexArray(0);
}

unsigned int VertexArray::GetIndexCount() const {
    return indexCount;
}