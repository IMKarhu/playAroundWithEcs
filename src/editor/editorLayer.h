#pragma once
#include "layer.h"
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
    void event(const Event &event) override;
private:
    const Renderer &m_renderer;
    Window &m_window;
};
