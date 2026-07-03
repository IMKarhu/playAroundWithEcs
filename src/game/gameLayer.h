#pragma once
#include "layer.h"
#include "ecsImpl/ecs.h"
#include "renderSystem.h"
#include "assetManager.h"
#include <memory>

class Renderer;

class GameLayer : public Layer
{
public:
    GameLayer(Renderer &renderer);

    void attach() override;
    void detach() override;
    void update(float dt) override;
    void event(const Event &event) override;
private:
    std::unique_ptr<RenderSystem> m_rendersystem;
    std::unique_ptr<AssetManager> m_assetmanager;
    Renderer &m_renderer;
    Ecs m_ecs;
};
