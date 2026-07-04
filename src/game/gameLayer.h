#pragma once
#include "layer.h"
#include "ecsImpl/ecs.h"
#include "assetManager.h"
#include <memory>

class Renderer;
class Window;
class FrameBufferManager;
class BasePass;
class ScreenPass;

class GameLayer : public Layer
{
public:
    GameLayer(Renderer& renderer, Window& window);

    void attach() override;
    void detach() override;
    void update(float dt) override;
    void event(const Event &event) override;
private:
    std::unique_ptr<BasePass> m_basepass;
    std::unique_ptr<ScreenPass> m_screenpass;
    std::unique_ptr<AssetManager> m_assetmanager;
    std::unique_ptr<FrameBufferManager> m_framebuffermanager;
    Renderer& m_renderer;
    Window& m_window;
    Ecs m_ecs;
};
