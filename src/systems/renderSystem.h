#pragma once
#include "ecs.h"
#include <memory>

class Window;
class Renderer;
class ImguiLayer;
class FrameBuffer;
class FrameBufferManager;

class RenderSystem
{
public:
    RenderSystem(Window& window);
    ~RenderSystem();

    void initialize();
    void update(float dt);
    void renderui();

private:
    Window& m_window;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<ImguiLayer> m_uilayer;
    std::shared_ptr<FrameBuffer> m_fb;
    std::shared_ptr<FrameBufferManager> m_framebufferManager;
};
