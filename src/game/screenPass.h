#pragma once
#include <cstdint>

namespace Lumos
{
    class Renderer;
    class FrameBufferManager;
}
class Ecs;

class ScreenPass
{
public:
    ScreenPass();
    ~ScreenPass();

    void update(const Ecs &ecs,
                Lumos::Renderer &renderer,
                uint32_t framebuffercolorattachment);
private:
};
