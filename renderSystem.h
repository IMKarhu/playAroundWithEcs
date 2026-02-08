#pragma once
#include <memory>

class Window;
class Renderer;
class ImguiLayer;
class Shader;

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
    std::unique_ptr<Shader> m_shader;
};
