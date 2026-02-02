#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <string>

class Window
{
public:
    Window();
    Window(uint32_t width, uint32_t height, std::string title);
    Window(const Window& win) = delete;
    Window& operator=(const Window& win) = delete;
    ~Window();

    GLFWwindow *handle() const;

    bool shouldClose() const;
    void pollEvents();
    void swapBuffers();
    int initWindow(uint32_t width, uint32_t height, std::string title);
private:
    GLFWwindow *m_window = nullptr;
    uint32_t m_width = 0;
    uint32_t m_height = 0;
    std::string m_title = "";

    static void framebufferCallback(GLFWwindow *window, int width, int height);
};
