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

    // This probably should take in imgui window flags as well.
    static void beginWindow(const char* name, bool *open);
    static void endWindow();
private:
    Window& m_window;
};
