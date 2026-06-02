#pragma once
#include "framebuffer.h"
#include <memory>

#ifdef RENDERER_EXPORT
#define RENDERER_API __declspec(dllexport)
#else
#define RENDERER_API __declspec(dllimport)
#endif

class Window;
class Renderer;

class RENDERER_API ImguiLayer
{
public:
    ImguiLayer(Window& window);
    ~ImguiLayer();

    void begin();
    void end();
    void drawViewport(std::shared_ptr<FrameBuffer> framebuffer);
    void drawSceneGraph();
    void drawInspector();

    // This probably should take in imgui window flags as well.
    static void beginWindow(const char* name, bool *open);
    static void endWindow();
private:
    Window& m_window;
    uint32_t m_selectedEntity = -1;
};
