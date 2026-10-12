#include "../headers/Window.h"
#include <iostream>

Window::Window(int width, int height) {
	m_width = width;
	m_height = height;
	m_window = nullptr;
}

Window::~Window() {
	if (m_window) {
		glfwDestroyWindow(m_window);

	}
	glfwTerminate();
}

bool Window::initialize(const char* title) {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW\n";
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(m_width, m_height, title, nullptr, nullptr);
	if (!m_window) {
		std::cerr << "Failed to create window\n";
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to init GLAD\n";
		return false;
	}

	glViewport(0, 0, m_width, m_height);
	return true;
}

bool Window::shouldClose() {
	return glfwWindowShouldClose(m_window);
}

void Window::update() {
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

GLFWwindow* Window::getGLFWWindow() const {
	return m_window;
}
