#include "../headers/Window.h"
#include <iostream>

// bn-set el ab3ad wel pointer b null f el awel
Window::Window(int width, int height) {
    this->width = width;
    this->height = height;
    window = nullptr;
}

// clean up lma ne2fel el le3ba
Window::~Window() {
    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

bool Window::initialize(const char* title) {
    // bn-init glfw el awel
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        return false;
    }

    // opengl version 4.3 core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // bn-create el window
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    // glad mas2ool y-load functions opengl
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD\n";
        return false;
    }

    // 7agm el rendering view 3ala el shasha
    glViewport(0, 0, width, height);
    return true;
}

// check lw el user das X aw alt+f4
bool Window::shouldClose() {
    return glfwWindowShouldClose(window);
}

// swap buffers w poll input events kol frame
void Window::update() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

GLFWwindow* Window::getGLFWWindow() const {
    return window;
}