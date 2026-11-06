#pragma once

// opengl loaders wel window management
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// class el window elly sha3'ala feeh el le3ba
class Window {
private:
    GLFWwindow* window; // pointer el window bta3 glfw
    int width;          // 3ard el shasha
    int height;         // tool el shasha

public:
    Window(int width, int height);
    ~Window();

    // bn-setup glfw w glad w nfta7 el window
    bool initialize(const char* title);

    // check law el user 3ayez ye2fel
    bool shouldClose();

    // swap buffers w poll events kol frame
    void update();

    // getter 3ashan n-bind el callbacks bta3et el input
    GLFWwindow* getGLFWWindow() const;
};