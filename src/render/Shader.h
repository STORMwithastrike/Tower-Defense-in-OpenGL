#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader {
public:
    GLuint ID;

    Shader(const char* vertexPath, const char* fragmentPath);
    void Use();

    void SetMat4(const std::string& name, const glm::mat4& mat);
    void SetFloat(const std::string& name, float value);
    void SetVec4(const std::string& name, float r, float g, float b, float a);

private:
    GLuint compileShader(GLenum type, const char* source);
};