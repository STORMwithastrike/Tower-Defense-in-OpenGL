#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader {
public:
    GLuint ID;

    Shader();
    Shader(const char* vertexPath, const char* fragmentPath);
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc, bool fromMemory);
    ~Shader();

    void Use() const;
    bool IsValid() const { return ID != 0; }

    void SetInt(const std::string& name, int value);
    void SetFloat(const std::string& name, float value);
    void SetVec2(const std::string& name, const glm::vec2& value);
    void SetVec3(const std::string& name, const glm::vec3& value);
    void SetVec4(const std::string& name, float r, float g, float b, float a);
    void SetVec4(const std::string& name, const glm::vec4& value);
    void SetMat4(const std::string& name, const glm::mat4& mat);

private:
    GLuint compileShader(GLenum type, const char* source);
    void linkProgram(GLuint vertexShader, GLuint fragmentShader);
};
