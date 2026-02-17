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
    uint32_t width() const;
    uint32_t height() const;

    bool shouldClose() const;
    void pollEvents();
    void swapBuffers();
    void makeCtxCurrent(GLFWwindow *ctx);
    int initWindow(uint32_t width, uint32_t height, std::string title);
    void uiExperiment();
private:
    GLFWwindow *m_window = nullptr;
    uint32_t m_width = 0;
    uint32_t m_height = 0;
    std::string m_title = "";

    static void framebufferCallback(GLFWwindow *window, int width, int height);
    static void GLAPIENTRY DebugMessageCallback(GLenum source,
            GLenum type,
            GLuint id,
            GLenum severity,
            GLsizei length,
            const GLchar* message,
            const void* userParam);
};
