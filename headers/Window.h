#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
private:
    GLFWwindow* window;
    int width;
    int height;

public:
    Window(int width, int height);
    ~Window();

    bool initialize(const char* title);
    bool shouldClose();
    void update();
    GLFWwindow* getGLFWWindow() const;
};