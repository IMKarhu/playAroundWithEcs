#pragma once
#include "renderer.h"
#include <vector>
#include <memory>

class Window;
class FrameBuffer;
class Shader;

class GLRenderer : public Renderer
{
public:
    GLRenderer(Window& window);
    ~GLRenderer() override;

    void initialize(std::vector<Entity>& entities) override;
    void beginFrame() override;
    void renderScene(float dt) override;
    void endFrame() override;
    FrameBuffer &viewportfb() override;
private:
    Window& m_window;
    std::unique_ptr<FrameBuffer> m_viewportfb;
    std::unique_ptr<Shader> m_shader;
};
