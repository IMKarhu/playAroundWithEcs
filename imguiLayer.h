#pragma once
#include <memory>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Window;
class Renderer;

class ImguiLayer
{
public:
    ImguiLayer(Window& window);
    ~ImguiLayer();

    void begin();
    void end();
    void drawViewport(std::unique_ptr<Renderer>& renderer);
private:
    Window& m_window;
};
