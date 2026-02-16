#include "window.h"
#include <print>
#include <iostream>

Window::Window()
    :m_width(600)
    ,m_height(400)
    ,m_title("Ecs testing")
{
    initWindow(m_width, m_height, m_title);
}

Window::Window(uint32_t width, uint32_t height, std::string title)
    :m_width(width)
    ,m_height(height)
    ,m_title(title)
{
    initWindow(m_width, m_height, m_title);
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
};

GLFWwindow *Window::handle() const
{
    return m_window;
}

uint32_t Window::width() const
{
    return m_width;
}

uint32_t Window::height() const
{
    return m_height;
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(m_window);
}

void Window::pollEvents()
{
    glfwPollEvents();
}

void Window::swapBuffers()
{
    glfwSwapBuffers(m_window);
}

void Window::makeCtxCurrent(GLFWwindow *ctx)
{
    glfwMakeContextCurrent(ctx);
}

int Window::initWindow(uint32_t width, uint32_t height, std::string title)
{
    int result;
    if(!glfwInit()) {
        std::println("you done fucked up");
        result = -1;
        return result;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if(!m_window) {
        std::println("failed to create window context");
        glfwTerminate();
        result = -1;
        return result;
    }

    glfwMakeContextCurrent(m_window);
    result = gladLoadGL();
    if(result == 1) {
        std::println("success on loading glad");
    }else {
        std::println("failed to load glad");
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(DebugMessageCallback, nullptr);
    glDebugMessageControl(
            GL_DONT_CARE,
            GL_DONT_CARE,
            GL_DEBUG_SEVERITY_NOTIFICATION,
            0, nullptr,
            GL_FALSE
    );

    std::cout << "GL Version: " << glGetString(GL_VERSION) << std::endl;

    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);
    glViewport(0, 0, w, h);

    glfwSetFramebufferSizeCallback(m_window, framebufferCallback);

    return result;
}

void Window::framebufferCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void GLAPIENTRY Window::DebugMessageCallback(GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam)
{
    std::println("OpenGL Error: type: {} message: {}", type, message);
}
