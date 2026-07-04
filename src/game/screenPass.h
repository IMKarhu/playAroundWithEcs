#pragma once

class Renderer;
class Ecs;
class AssetManager;
class FrameBufferManager;

class ScreenPass
{
public:
    ScreenPass();
    ~ScreenPass();

    void update(float dt,
                const Ecs &ecs,
                Renderer &renderer,
                const AssetManager& assetmanager,
                FrameBufferManager& framebuffermanager);
private:
};
