#pragma once
#include "layer.h"
#include "basePass.h"
#include "screenPass.h"
#include "lightingPass.h"
#include "framebufferManager.h"
#include <memory>


class Ecs;
namespace Lumos
{
    class Renderer;
    class Window;
    class AssetManager;
    class Scene;
}

class GameLayer : public Lumos::Layer
{
public:
    GameLayer(Lumos::Renderer& renderer, Lumos::AssetManager& assetmanager, const Lumos::Window& window);

    void attach() override;
    void detach() override;
    void update(float dt, const std::shared_ptr<Lumos::Scene>& scene) override;
    void event(Lumos::Event &event, const std::shared_ptr<Lumos::Scene>& scene) override;
    //just for testing purposes so I can take out all matrice math from pass update
    void updateTransform(float dt, Ecs& ecs);
private:
    std::unique_ptr<BasePass> m_basepass;
    std::unique_ptr<ScreenPass> m_screenpass;
    std::unique_ptr<LightingPass> m_lightingpass;
    std::unique_ptr<Lumos::FrameBufferManager> m_framebuffermanager;
    Lumos::Renderer& m_renderer;
    const Lumos::Window& m_window;
    Lumos::AssetManager& m_assetmanager;
};
