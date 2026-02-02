#include "window.h"

#include <print>

Window::Window()
    :m_width(600)
    ,m_height(400)
    ,m_title("Ecs testing")
{
    if(!glfwInit()) {
        std::println("you done fucked up");
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
}

Window::Window(uint32_t width, uint32_t height, std::string title)
    :m_width(width)
    ,m_height(height)
    ,m_title(title)
{
    if(!glfwInit()) {
        std::println("you done fucked up");
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
}

Window::~Window()
{
    glfwTerminate();
};

GLFWwindow *Window::handle() const
{
    return m_window;
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(m_window);
}

void Window::pollEvents()
{
    glfwPollEvents();
}
