#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>
#include <platforms.h>

#ifdef _WIN32
#ifdef CORE_EXPORT
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif
#else
#define CORE_API
#endif

class CORE_API Window
{
public:
    Window();
    Window(Platform platform, uint32_t width, uint32_t height, std::string title);
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
    void initWindow(Platform platform, uint32_t width, uint32_t height, std::string title);
private:
    GLFWwindow *m_window = nullptr;
    uint32_t m_width = 0;
    uint32_t m_height = 0;
    std::string m_title = "";

    static void framebufferCallback(GLFWwindow *window, int width, int height);
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
};
