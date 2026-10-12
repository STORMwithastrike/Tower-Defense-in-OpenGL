// Minimal setup test: opens an OpenGL 3.3 core window (also valid on macOS) and clears the screen.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <cstdio>

static void framebufferSizeCallback(GLFWwindow*, int w, int h) {
    glViewport(0, 0, w, h);
}

int main() {
    if (!glfwInit()) {
        std::fprintf(stderr, "Failed to initialise GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // required on macOS
#endif

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Tower Defense", nullptr, nullptr);
    if (!window) {
        std::fprintf(stderr, "Failed to create window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::fprintf(stderr, "Failed to initialise GLAD\n");
        return -1;
    }
    std::printf("OpenGL %s | %s\n", glGetString(GL_VERSION), glGetString(GL_RENDERER));

    const glm::vec3 clearColor(0.10f, 0.12f, 0.18f);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);

        glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
