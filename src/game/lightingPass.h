#pragma once

namespace Lumos
{
    class Renderer;
    class FrameBufferManager;
}
class Ecs;

class LightingPass
{
public:
    LightingPass();
    ~LightingPass();

    void update(const Ecs &ecs, Lumos::Renderer &renderer, Lumos::FrameBufferManager& framebuffermanager);
private:
};

