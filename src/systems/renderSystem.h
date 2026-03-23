#pragma once
#include "ecs.h"
#include <memory>

class Window;
class Renderer;
class ImguiLayer;
class FrameBuffer;
class FrameBufferManager;
class Scene;

class RenderSystem
{
public:
    RenderSystem(Window& window);
    ~RenderSystem();

    void initialize(std::shared_ptr<Scene>& scene);
    void update(float dt, std::shared_ptr<Scene>& scene);
    void renderui(std::shared_ptr<Scene>& scene);

private:
    Window& m_window;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<ImguiLayer> m_uilayer;
    std::shared_ptr<FrameBuffer> m_fb;
    std::shared_ptr<FrameBufferManager> m_framebufferManager;
};
