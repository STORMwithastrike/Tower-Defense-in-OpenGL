#pragma once
#include <glad/glad.h>
#include <vector>

class VertexArray {
public:
    // Constructor takes dynamic arrays of vertices and indices
    VertexArray(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);

    // Destructor to clean up GPU memory
    ~VertexArray();

    // Bind and Unbind the VAO for drawing
    void Bind() const;
    void Unbind() const;

    // Getter so the Renderer knows how many indices to draw
    unsigned int GetIndexCount() const;

private:
    GLuint VAO, VBO, EBO;
    unsigned int indexCount;
};