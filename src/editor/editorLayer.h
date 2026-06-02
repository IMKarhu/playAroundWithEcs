#pragma once
#include "layer.h"
#include "renderer/renderSystem.h"
#include <memory>


class Renderer;
class Window;

class EditorLayer : public Layer
{
public:
    EditorLayer(const Renderer &renderer, Window &window);
    ~EditorLayer();

    void attach() override;
    void detach() override;
    void update(float dt) override;
    void event(Event &event) override;
private:
    std::unique_ptr<RenderSystem> m_rendersystem;
    const Renderer &m_renderer;
    Window &m_window;
};
