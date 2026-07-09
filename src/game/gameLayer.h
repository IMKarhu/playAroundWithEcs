#pragma once
#include "layer.h"
#include "ecsImpl/ecs.h"
#include <memory>

class Renderer;
class Window;
class FrameBufferManager;
class BasePass;
class ScreenPass;
class LightingPass;
namespace Lumos
{
    class AssetManager;
}

struct Voxel {
    float density;
};

// const int CHUNKSIZE = 32;
// Voxel volumeData[CHUNKSIZE * CHUNKSIZE * CHUNKSIZE];

class GameLayer : public Layer
{
public:
    GameLayer(Renderer& renderer, Lumos::AssetManager& assetmanager, Window& window);

    void attach() override;
    void detach() override;
    void update(float dt) override;
    void event(const Event &event) override;
    //just for testing purposes so I can take out all matrice math from pass update
    void updateTransform(float dt);
private:
    std::unique_ptr<BasePass> m_basepass;
    std::unique_ptr<ScreenPass> m_screenpass;
    std::unique_ptr<LightingPass> m_lightingpass;
    std::unique_ptr<FrameBufferManager> m_framebuffermanager;
    Renderer& m_renderer;
    Window& m_window;
    Lumos::AssetManager& m_assetmanager;
    Ecs m_ecs;


};
