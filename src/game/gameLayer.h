#pragma once
#include "layer.h"
#include "ecsImpl/ecs.h"
#include "renderSystem.h"
#include <memory>

class Renderer;

class GameLayer : public Layer
{
public:
    GameLayer(const Renderer &renderer);

    void attach() override;
    void detach() override;
    void update(float dt) override;
    void event(Event &event) override;
private:
    std::unique_ptr<RenderSystem> m_rendersystem;
    const Renderer &m_renderer;
    Ecs m_ecs;
};
