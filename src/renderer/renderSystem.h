#pragma once

#ifdef RENDERER_EXPORT
#define RENDERER_API __declspec(dllexport)
#else
#define RENDERER_API __declspec(dllimport)
#endif

class Renderer;
class Ecs;

class RENDERER_API RenderSystem
{
public:
    RenderSystem();
    ~RenderSystem();

    void initialize(const Ecs &ecs, const Renderer &renderer);
    void update(float dt, const Ecs &ecs, const Renderer &renderer);
private:
};
