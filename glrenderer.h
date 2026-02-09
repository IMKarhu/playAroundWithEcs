#pragma once
#include "renderer.h"
#include <vector>
#include <memory>

class Window;
class FrameBuffer;

class GLRenderer : public Renderer
{
public:
    GLRenderer(Window& window);

    void initialize(std::vector<Entity>& entities) override;
    void beginFrame() override;
    void renderScene() override;
    void endFrame() override;
    FrameBuffer &viewportfb() override;
private:
    Window& m_window;
    std::unique_ptr<FrameBuffer> m_viewportfb;
};
