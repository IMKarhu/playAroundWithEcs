#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>
#include <functional>
#include "events.h"
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

namespace Lumos
{
    class CORE_API Window
    {
        public:
            using EventCallbackFunction = std::function<void(Event&)>;
            Window();
            Window(Platform platform, uint32_t width, uint32_t height, std::string title);
            Window(const Window& win) = delete;
            Window& operator=(const Window& win) = delete;
            ~Window();

            GLFWwindow *handle() const;
            uint32_t width() const;
            uint32_t height() const;

            bool shouldClose() const;
            void setWindowShouldClose(bool close);
            void setWindowTitle(const std::string& title);
            void pollEvents();
            void swapBuffers();
            void makeCtxCurrent(GLFWwindow *ctx);
            void setEventCallback(const EventCallbackFunction& callback);
            void initWindow(Platform platform, uint32_t width, uint32_t height, std::string title);
        private:
            GLFWwindow *m_window = nullptr;
            struct WindowData
            {
                uint32_t m_width = 0;
                uint32_t m_height = 0;
                std::string m_title = "";
                EventCallbackFunction m_eventcallback;
            } m_windowdata;

            static void framebufferCallback(GLFWwindow *window, int width, int height);
            static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    };
}
