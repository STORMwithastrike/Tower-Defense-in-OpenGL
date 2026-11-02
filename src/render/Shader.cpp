#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader() : ID(0) {}

Shader::Shader(const char* vertexPath, const char* fragmentPath) : ID(0) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        if (!vShaderFile.is_open()) {
            std::cerr << "Shader ERROR: Could not open vertex shader at " << vertexPath << std::endl;
        }
        if (!fShaderFile.is_open()) {
            std::cerr << "Shader ERROR: Could not open fragment shader at " << fragmentPath << std::endl;
        }

        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::exception& e) {
        std::cerr << "Shader file read exception: " << e.what() << std::endl;
    }

    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode.c_str());
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());
    linkProgram(vertexShader, fragmentShader);
}

Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc, bool fromMemory) : ID(0) {
    (void)fromMemory;
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSrc.c_str());
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSrc.c_str());
    linkProgram(vertexShader, fragmentShader);
}

Shader::~Shader() {
    if (ID != 0) {
        glDeleteProgram(ID);
    }
}

GLuint Shader::compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "Shader Compile Error (" << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT")
                  << "):\n" << infoLog << std::endl;
    }
    return shader;
}

void Shader::linkProgram(GLuint vertexShader, GLuint fragmentShader) {
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    int success;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(ID, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "Shader Link Error:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Use() const {
    if (ID != 0) {
        glUseProgram(ID);
    }
}

void Shader::SetInt(const std::string& name, int value) {
    GLint location = glGetUniformLocation(ID, name.c_str());
    if (location != -1) glUniform1i(location, value);
}

void Shader::SetFloat(const std::string& name, float value) {
    GLint location = glGetUniformLocation(ID, name.c_str());
    if (location != -1) glUniform1f(location, value);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) {
    GLint location = glGetUniformLocation(ID, name.c_str());
    if (location != -1) glUniform2f(location, value.x, value.y);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) {
    GLint location = glGetUniformLocation(ID, name.c_str());
    if (location != -1) glUniform3f(location, value.x, value.y, value.z);
}

void Shader::SetVec4(const std::string& name, float r, float g, float b, float a) {
    GLint location = glGetUniformLocation(ID, name.c_str());
    if (location != -1) glUniform4f(location, r, g, b, a);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) {
    GLint location = glGetUniformLocation(ID, name.c_str());
    if (location != -1) glUniform4f(location, value.r, value.g, value.b, value.a);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) {
    GLint location = glGetUniformLocation(ID, name.c_str());
    if (location != -1) glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}
