#pragma once
#include "renderer.h"
#include <memory>

class Window;
class FrameBuffer;
class Shader;
class Scene;

class GLRenderer : public Renderer
{
public:
    GLRenderer(Window& window);
    ~GLRenderer() override;

    void initialize() override;
    void beginFrame() override;
    void renderScene(float dt, uint32_t width, uint32_t height) override;
    void endFrame() override;
private:
    Window& m_window;
    std::unique_ptr<Shader> m_shader;
};
