#pragma once
#include <cstdint>

class Renderer;
class Ecs;
class FrameBufferManager;

class ScreenPass
{
public:
    ScreenPass();
    ~ScreenPass();

    void update(const Ecs &ecs,
                Renderer &renderer,
                uint32_t framebuffercolorattachment);
private:
};
