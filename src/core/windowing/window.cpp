#include "window.h"
#include "events.h"
#include <print>

Window::Window()
    :m_width(600)
    ,m_height(400)
    ,m_title("Ecs testing")
{
    Platform platform;
    initWindow(platform, m_width, m_height, m_title);
}

Window::Window(Platform platform, uint32_t width, uint32_t height, std::string title)
    :m_width(width)
    ,m_height(height)
    ,m_title(title)
{
    initWindow(platform, m_width, m_height, m_title);
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

void Window::setWindowShouldClose(bool close)
{
    glfwSetWindowShouldClose(m_window, GLFW_TRUE);
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

void Window::initWindow(Platform platform, uint32_t width, uint32_t height, std::string title)
{
    if(!glfwInit()) {
        std::println("Failed to initialize GLFW");
        return ;
    }

    if (platform.isWayland && glfwPlatformSupported(GLFW_PLATFORM_WAYLAND)) {
        glfwWindowHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
    } else {
        std::println("no wayland support detected");
    }

    if (platform.api == GraphicsApi::VULKAN) {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    } else {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
    }

    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if(!m_window) {
        std::println("failed to create window context");
        glfwTerminate();
        return ;
    }

    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, framebufferCallback);
    glfwSetKeyCallback(m_window, keyCallback);
}

void Window::framebufferCallback(GLFWwindow *window, int width, int height)
{
    ResizeEvent event(width, height);
    EventDispatcher::sendEvent(event);
}

void Window::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    KeyEvent event(key, action, mods);
    EventDispatcher::sendEvent(event);
}
