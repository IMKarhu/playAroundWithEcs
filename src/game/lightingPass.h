#pragma once

class Renderer;
class Ecs;
class FrameBufferManager;

class LightingPass
{
public:
    LightingPass();
    ~LightingPass();

    void update(const Ecs &ecs, Renderer &renderer, FrameBufferManager& framebuffermanager);
private:
};

