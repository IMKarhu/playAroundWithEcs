#pragma once

#ifdef _WIN32
    #ifdef RENDERER_EXPORT
    #define RENDERER_API __declspec(dllexport)
    #else
    #define RENDERER_API __declspec(dllimport)
    #endif
#else
    #define RENDERER_API
#endif

class Renderer;
class Ecs;
class AssetManager;

class RENDERER_API RenderSystem
{
public:
    RenderSystem();
    ~RenderSystem();

    void initialize(const Ecs &ecs, const Renderer &renderer);
    void update(float dt, const Ecs &ecs, Renderer &renderer, const AssetManager& assetmanager);
private:
};
